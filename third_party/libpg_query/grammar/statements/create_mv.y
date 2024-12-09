
CreateMatViewStmt:
		CREATE_P OptNoLog MATERIALIZED VIEW create_mv_target AS SelectStmt opt_with_data
				{
					PGCreateTableAsStmt *ctas = makeNode(PGCreateTableAsStmt);
					ctas->query = $6;
					ctas->into = $4;
					ctas->relkind = PG_OBJECT_TABLE;
					ctas->is_select_into = false;
					ctas->onconflict = PG_ERROR_ON_CONFLICT;
					/* cram additional flags into the PGIntoClause */
					$5->rel->relpersistence = $2;
					$5->skipData = !($8);
					$$ = (PGNode *) ctas;
				}

		| CREATE_P OptNoLog MATERIALIZED VIEW IF_P NOT EXISTS create_mv_target AS SelectStmt opt_with_data
				{
					PGCreateTableAsStmt *ctas = makeNode(PGCreateTableAsStmt);
					ctas->query = $9;
					ctas->into = $7;
					ctas->relkind = PG_OBJECT_TABLE;
					ctas->is_select_into = false;
					ctas->onconflict = PG_IGNORE_ON_CONFLICT;
					/* cram additional flags into the PGIntoClause */
					$8->rel->relpersistence = $2;
					$8->skipData = !($11);
					$$ = (PGNode *) ctas;
				}
		;

OptNoLog:	UNLOGGED					{ $$ = RELPERSISTENCE_UNLOGGED; }
			| /*EMPTY*/					{ $$ = RELPERSISTENCE_PERMANENT; }
		;

create_mv_target:
		qualified_name opt_column_list OptWith OnCommitOption opt_reloptions
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

