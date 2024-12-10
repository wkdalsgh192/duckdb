
/*****************************************************************************
 *
 *		QUERY :
 *				REFRESH MATERIALIZED VIEW qualified_name
 *
 *****************************************************************************/
RefreshMatViewStmt:
		REFRESH MATERIALIZED VIEW qualified_name
			{
				PGRefreshMatViewStmt *n = makeNode(PGRefreshMatViewStmt);

				n->relation = $4;
				n->relkind = PG_OBJECT_TABLE;
				$$ = (PGNode *) n;
			}
		;
