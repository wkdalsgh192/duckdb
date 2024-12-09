
CreateMatViewStmt:
		CREATE_P MATERIALIZED VIEW create_mv_target AS SelectStmt opt_with_data
				{
					PGMaterializedViewStmt *ctas = makeNode(PGMaterializedViewStmt);
					ctas->query = $6;
					ctas->into = $4;
					ctas->relkind = PG_OBJECT_TABLE;
					ctas->is_select_into = false;
					ctas->onconflict = PG_ERROR_ON_CONFLICT;
					/* cram additional flags into the PGIntoClause */
					$4->rel->relpersistence = *($2);
					$4->skipData = !($7);
					$$ = (PGNode *) ctas;
				}

		| CREATE OptNoLog MATERIALIZED VIEW IF_P NOT EXISTS create_mv_target AS SelectStmt opt_with_data
				{
					PGMaterializedViewStmt *ctas = makeNode(PGMaterializedViewStmt);
					ctas->query = $9;
					ctas->into = $7;
					ctas->relkind = PG_OBJECT_TABLE;
					ctas->is_select_into = false;
					ctas->onconflict = PG_IGNORE_ON_CONFLICT;
					/* cram additional flags into the PGIntoClause */
					$7->rel->relpersistence = $2;
					$7->skipData = !($10);
					$$ = (PGNode *) ctas;
				}
		;

create_mv_target:
		qualified_name opt_column_list OptWith OnCommitOption
				{
					$$ = makeNode(PGIntoClause);
					$$->rel = $1;
					$$->colNames = $2;
					$$->options = $3;
					$$->onCommit = $4;
					$$->viewQuery = NULL;
					$$->skipData = false;		/* might get changed later */
				}
		;

