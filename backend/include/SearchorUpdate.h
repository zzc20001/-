#ifndef SEARCH_H
#define SEARCH_H
#include<crow.h>
#include<bits/stdc++.h>
#include "database.h"

crow::response search_items(const std::string& query, std::unique_ptr<sql::Connection>& con);
void update(const int& id,std::unique_ptr<sql::Connection>& con,const std::map<std::string, std::string>& fields, const std::string& imagePath);

#endif
