
class TablePlan {
  TableInfo table_info;
  Transaction transaction;

  TablePlan(string table_name, Transaction tx) : transaction(tx) {
    table_info = MetadataManager.instance().table_info(table_name);
    stat_info  = MetadataManager.instance().stat_info(table_name);
  }
  
  Scan open() {
    TableScan scan(table_info, 
    return 
  }
};

class ProductPlan {
};

class SelectPlan {
};

class ProjectPlan {
};

class QueryPlanner {
  Plan create_plan(QueryData data, Transaction tx) {
    vector<Plan> plans;
    for (int i = 0; i < data.tables.size(); i++) {
      string table_name = data.tables[i];
      TablePlan table_plan(table_name);
      plans.push_back(table_plan, tx);
    }

    Plan p = plans.front();
    for (int i = 1; i < plans.size(); i++) {
      p = ProductPlan(p, plans[i]);
    }

    p = SelectPlan(p, data.pred);
    p = ProjectPlan(p, data.fields);
    
    return p;
  }
};
