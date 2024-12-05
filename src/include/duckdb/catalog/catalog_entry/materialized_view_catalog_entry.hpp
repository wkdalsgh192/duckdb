//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/catalog/catalog_entry/view_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/standard_entry.hpp"
#include "duckdb/parser/statement/select_statement.hpp"
#include "duckdb/common/types.hpp"
#include "duckdb/common/vector.hpp"

namespace duckdb {

class DataTable;
struct CreateMaterializedViewInfo;

//! A view catalog entry
class MaterializedViewCatalogEntry : public StandardEntry {
public:
	static constexpr const CatalogType Type = CatalogType::MATERIALIZED_VIEW_ENTRY;
	static constexpr const char *Name = "materialized view";

public:
	//! Create a real TableCatalogEntry and initialize storage for it
	MaterializedViewCatalogEntry(Catalog &catalog, SchemaCatalogEntry &schema, CreateMaterializedViewInfo &info);

	//! The query of the view
	unique_ptr<SelectStatement> query;
	//! The SQL query (if any)
	string sql;
	//! The set of aliases associated with the view
	vector<string> aliases;
	//! The returned types of the view
	vector<LogicalType> types;
	//! The returned names of the view
	vector<string> names;
	//! The comments on the columns of the view: can be empty if there are no comments
	vector<Value> column_comments;

public:
	unique_ptr<CreateInfo> GetInfo() const override;

	unique_ptr<CatalogEntry> AlterEntry(ClientContext &context, AlterInfo &info) override;

	unique_ptr<CatalogEntry> Copy(ClientContext &context) const override;

	string ToSQL() const override;

private:
	void Initialize(CreateMaterializedViewInfo &info);
};
} // namespace duckdb
