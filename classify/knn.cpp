#include <iostream>
#include <utility>
#include "knn.h"

using std::pair;
using std::unordered_map;
using std::multimap;
using std::string;
using std::vector;
using std::shared_ptr;
using namespace KNN::internal;

string KNN::classify(Document & query, shared_ptr<Index> index, size_t k)
{
    multimap<double, string> ranking = index->search(query);
    return findNN(ranking, k);
}

string KNN::internal::findNN(const multimap<double, string> & ranking, size_t k)
{
    unordered_map<string, size_t> counts;
    size_t numResults = 0;
    for(auto result = ranking.rbegin(); result != ranking.rend() && numResults++ != k; ++result)
    {
        size_t space = result->second.find_first_of(" ") + 1;
        string category = result->second.substr(space, result->second.size() - space);
        ++counts[category];
    }

    string best = "[no results]";
    size_t high = 0;
    for(auto & count: counts)
    {
        if(count.second > high)
        {
            best = count.first;
            high = count.second;
        }
    }

    return best;
}

string KNN::classify(Document & query, vector<shared_ptr<Index>> indexes, vector<double> weights, size_t k)
{
    // make sure weights sum to 1.0
    double sum = 0.0;
    for(double weight: weights)
        sum += weight;

    if(sum != 1.0)
    {
        std::cerr << "[KNN::classify]: weights in ensemble do not add to 1.0" << std::endl;
        return "";
    }

    // create a vector of normalized results for each index
    vector<unordered_map<string, double>> results;
    for(shared_ptr<Index> & ptr: indexes)
    {
        multimap<double, string> result = ptr->search(query);
        unordered_map<string, double> normalized = normalize(result);
        results.push_back(normalized);
    }

    // iterate over the elements in the first hashtable (they should all be the same),
    //   and add the interpolated values into the final ranking
    multimap<double, string> ranking;
    for(auto & rank: results[0])
    {
        double score = 0.0;
        for(size_t i = 0; i < results.size(); ++i)
            score += (results[i][rank.first] * weights[i]);
        ranking.insert(make_pair(score, rank.first));
    }

    return findNN(ranking, k);
}

unordered_map<string, double>
KNN::internal::normalize(const multimap<double, string> & scores)
{
    unordered_map<string, double> normalized;

    // we know the first element has the highest score and the last element has
    //  the lowest score
    return normalized;
}