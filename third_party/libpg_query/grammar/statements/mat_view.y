/*****************************************************************************
 *
 *	QUERY:
 *		CREATE [ OR REPLACE ] MATERIALIZED VIEW <viewname> '('target-list ')'
 *			AS <query>
 *
 *****************************************************************************/
MatViewStmt: 
    CREATE_P MATVIEW qualified_name opt_column_list
    AS SelectStmt
    {
        PGMaterializedViewStmt *n = makeNode(PGMaterializedViewStmt);
        n->view = $3;
        n->aliases = $4;
        n->query = $6;
        n->onconflict = PG_ERROR_ON_CONFLICT;
        $$ = (PGNode *) n;
    }
| CREATE_P OR REPLACE MATVIEW qualified_name opt_column_list
    AS SelectStmt
    {
        PGMaterializedViewStmt *n = makeNode(PGMaterializedViewStmt);
        n->view = $5;
        n->aliases = $6;
        n->query = $8;
        n->onconflict = PG_REPLACE_ON_CONFLICT;
        $$ = (PGNode *) n;
    }
;
