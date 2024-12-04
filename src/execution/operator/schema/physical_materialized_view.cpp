#include "duckdb/catalog/catalog.hpp"
#include "duckdb/execution/operator/schema/physical_materialized_view.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// Source
//===--------------------------------------------------------------------===//
SourceResultType PhysicalCreateMatView::GetData(ExecutionContext &context, DataChunk &chunk,
                                             OperatorSourceInput &input) const {
	auto &catalog = Catalog::GetCatalog(context.client, info->catalog);
	catalog.CreateView(context.client, *info);

	return SourceResultType::FINISHED;
}

} // namespace duckdb
