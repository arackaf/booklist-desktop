#pragma once

#include <string>
#include <functional>

class GraphqlUrlGenerator
{
public:
    GraphqlUrlGenerator();

    static std::string GenerateGraphqlUrl(const std::string &, const std::string &);
    static std::function<std::string(const std::string &)> MakeUrlGenerator(const std::string &);

private:
    static std::string MasterBase;
};
