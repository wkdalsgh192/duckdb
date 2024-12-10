#include "duckdb/parser/parsed_data/create_table_info.hpp"
#include "duckdb/parser/statement/create_statement.hpp"
#include "duckdb/parser/transformer.hpp"

namespace duckdb {

unique_ptr<CreateStatement>
Transformer::TransformRefreshMaterializedView(duckdb_libpgquery::PGRefreshMatViewStmt &stmt) {

	auto result = make_uniq<CreateStatement>();
	auto info = make_uniq<CreateTableInfo>();

	info->table = stmt.relation->relname;
	info->type = CatalogType::MATERIALIZED_VIEW_ENTRY;
	info->on_conflict = OnCreateConflict::REPLACE_ON_CONFLICT;

	result->info = std::move(info);
	return result;
}

} // namespace duckdb
