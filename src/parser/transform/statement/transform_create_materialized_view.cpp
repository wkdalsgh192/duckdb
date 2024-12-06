#include "duckdb/parser/statement/create_statement.hpp"
#include "duckdb/parser/transformer.hpp"
#include "duckdb/parser/parsed_data/create_materialized_view_info.hpp"

namespace duckdb {

unique_ptr<CreateStatement> Transformer::TransformCreateMaterializedView(duckdb_libpgquery::PGMaterializedViewStmt &stmt) {
    D_ASSERT(stmt.type == duckdb_libpgquery::T_PGMaterializedViewStmt);
    D_ASSERT(stmt.view);

    auto result = make_uniq<CreateStatement>();
    auto info = make_uniq<CreateMaterializedViewInfo>();

    auto qname = TransformQualifiedName(*stmt.view);
    info->catalog = qname.catalog;
    info->schema = qname.schema;
    info->view_name = qname.name;
    info->temporary = !stmt.view->relpersistence;
    if (info->temporary && IsInvalidCatalog(info->catalog)) {
        info->catalog = TEMP_CATALOG;
    }
    info->on_conflict = TransformOnConflict(stmt.onconflict);

    info->query = TransformSelectStmt(*stmt.query, false);

    PivotEntryCheck("materialized_view");

    if (stmt.aliases && stmt.aliases->length > 0) {
        for (auto c = stmt.aliases->head; c != nullptr; c = lnext(c)) {
            auto val = PGPointerCast<duckdb_libpgquery::PGValue>(c->data.ptr_value);
            switch (val->type) {
            case duckdb_libpgquery::T_PGString: {
                info->aliases.emplace_back(val->val.str);
                break;
            }
            default:
                throw NotImplementedException("Materialized view projection type");
            }
        }
        if (info->aliases.empty()) {
            throw ParserException("Need at least one column name in CREATE MATERIALIZED VIEW projection list");
        }
    }

    if (stmt.options && stmt.options->length > 0) {
        throw NotImplementedException("MATERIALIZED VIEW options");
    }

    // Materialized views do not support WITH CHECK OPTION.
    if (stmt.withCheckOption != duckdb_libpgquery::PGViewCheckOption::PG_NO_CHECK_OPTION) {
        throw NotImplementedException("MATERIALIZED VIEW CHECK options");
    }

    result->info = std::move(info);
    return result;
}

} // namespace duckdb