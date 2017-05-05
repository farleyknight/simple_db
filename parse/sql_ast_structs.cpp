
/////////////
// Structs //
/////////////

struct SQLExpr {
  string expr;
  SQLExpr() { }
  SQLExpr(string e) : expr(e) { }
};

void print_expr(SQLExpr e) {
  cout << "expr = " << e.expr << endl;
}

struct SQLFieldName : SQLExpr {
  SQLFieldName(string e) : SQLExpr(e) { }
};

struct SQLConst : SQLExpr {
  SQLConst(string e) : SQLExpr(e) { }
};

struct SQLStringConst : SQLConst {
  SQLStringConst(string e) : SQLConst(e) { }
};

struct SQLIntConst : SQLConst {
  int value;
  SQLIntConst(string e) : SQLConst(e), value(std::atoi(e.c_str())) { }
};

struct SQLTerm {
  SQLExpr left, right;
  SQLTerm() { }
  SQLTerm(SQLExpr l, SQLExpr r) : left(l), right(r) { }
};

struct SQLPredicate {
  vector<SQLTerm> terms;
  SQLPredicate() { }
  SQLPredicate(SQLTerm t) {
    terms.push_back(t);
  }

  void conjoin(SQLPredicate p) {
    for (int i = 0; i < p.terms.size(); i++) {
      terms.push_back(p.terms[i]);
    }
  }
};

void print_predicate(SQLPredicate pred) {
  for (int i = 0; i < pred.terms.size(); i++) {
    cout << "left = " << pred.terms[i].left.expr << endl;
    cout << "right = " << pred.terms[i].right.expr << endl;
  }
}


struct SQLSelectQuery {
  vector<string> fields;
  vector<string> tables;
  SQLPredicate pred;
};

