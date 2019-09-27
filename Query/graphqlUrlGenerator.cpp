#include "graphqlUrlGenerator.h"
#include "query.h"
#include <functional>

GraphqlUrlGenerator::GraphqlUrlGenerator()
{

}

std::string GraphqlUrlGenerator::MasterBase = "";

std::string GraphqlUrlGenerator::GenerateGraphqlUrl(const std::string &query, const std::string &variables)
{
    return GraphqlUrlGenerator::MasterBase + encode(query) + /*"&operationName=" + operation*/ "&variables=" + encode(variables);
}

std::function<std::string(const std::string &)> GraphqlUrlGenerator::MakeUrlGenerator(const std::string &query)
{
    using namespace std::placeholders;

    return std::bind(GraphqlUrlGenerator::GenerateGraphqlUrl, query, _1);
}
