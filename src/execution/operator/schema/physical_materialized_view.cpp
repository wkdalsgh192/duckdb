#include "duckdb/catalog/catalog.hpp"
#include "duckdb/execution/operator/schema/physical_materialized_view.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// Source
//===--------------------------------------------------------------------===//
SourceResultType PhysicalCreateMatView::GetData(ExecutionContext &context, DataChunk &chunk,
                                             OperatorSourceInput &input) const {
	optional_ptr<TableCatalogEntry> table;
	auto &catalog = Catalog::GetCatalog(context.client, info->catalog);
	//// Parse analysis and query rewrite
	catalog.CreateMaterializedView(context.client, *info);
	//auto &schema = info->schema;
	//auto &catalog = schema;
	//catalog.CreateView()
	//catalog.CreateView()
	//catalog.CreateTable(catalog.GetCatalogTransaction(context.client), info->schema, *info);
	return SourceResultType::FINISHED;
}

} // namespace duckdb
