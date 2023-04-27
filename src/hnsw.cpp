#include "hnsw.h"

int Item::dist(Item &other, Key key, int size, int bound) const {
    return eval(key, value, other.value, size, bound);
}

HNSWGraph::HNSWGraph(int NN, int MN, int MNZ, int SN, int ML, Key key, int size, int bound) :
        NN(NN), MN(MN), MNZ(MNZ), SN(SN), ML(ML), key(key), size(size), bound(bound) {
    numItem = 0;
    enterNode = 0;
    layerEdgeLists.emplace_back();
}

void HNSWGraph::insert(Item &q) {
    int nid = static_cast<int>(items.size());
    numItem++;
    items.push_back(q);
    // sample layer
    int maxLayer = static_cast<int>(layerEdgeLists.size()) - 1;
    int l = 0;
    uniform_real_distribution<double> distribution(0.0, 1.0);
    while (l < ML && (1.0 / ML <= distribution(generator))) {
        l++;
        if (layerEdgeLists.size() <= l) layerEdgeLists.emplace_back();
    }
    if (nid == 0) {
        enterNode = nid;
        return;
    }
    // search up layer entrance
    int ep = enterNode;
    for (int i = maxLayer; i > l; i--) ep = searchLayer(q, ep, 1, i)[0];
    for (int i = min(l, maxLayer); i >= 0; i--) {
        int MM = l == 0 ? MNZ : MN;
        vector<int> neighbors = searchLayer(q, ep, SN, i);
        vector<int> selectedNeighbors = vector<int>(neighbors.begin(),
                                                    neighbors.begin() + min(int(neighbors.size()), NN));
        for (int n: selectedNeighbors) addEdge(n, nid, i);
        for (int n: selectedNeighbors) {
            if (layerEdgeLists[i][n].size() > MM) {
                vector<pair<double, int>> distPairs;
                for (int nn: layerEdgeLists[i][n])
                    distPairs.emplace_back(items[n].dist(items[nn], key, size, bound), nn);
                sort(distPairs.begin(), distPairs.end());
                layerEdgeLists[i][n].clear();
                for (int d = 0; d < min(int(distPairs.size()), MM); d++)
                    layerEdgeLists[i][n].push_back(distPairs[d].second);
            }
        }
        ep = selectedNeighbors[0];
    }
    if (l == layerEdgeLists.size() - 1) enterNode = nid;
}

vector<int> HNSWGraph::search(Item &q, int K) {
    int maxLayer = static_cast<int>(layerEdgeLists.size()) - 1;
    int ep = enterNode;
    for (auto l = maxLayer; l >= 1; l--) ep = searchLayer(q, ep, 1, l)[0];
    return searchLayer(q, ep, K, 0);
}

void HNSWGraph::addEdge(int st, int ed, int lc) {
    if (st == ed) return;
    layerEdgeLists[lc][st].push_back(ed);
    layerEdgeLists[lc][ed].push_back(st);
}

vector<int> HNSWGraph::searchLayer(Item &q, int ep, int ef, int lc) {
    unordered_set<int> isVisited;
    set<pair<double, int>> candidates;
    set<pair<double, int>> nearestNeighbors;


    double td = q.dist(items[ep], key, size, bound);
    candidates.insert(make_pair(td, ep));
    nearestNeighbors.insert(make_pair(td, ep));
    isVisited.insert(ep);
    while (!candidates.empty()) {
        auto ci = candidates.begin();
        candidates.erase(candidates.begin());
        int nid = ci->second;
        auto fi = nearestNeighbors.end();
        fi--;
        if (ci->first > fi->first) break;
        for (int ed: layerEdgeLists[lc][nid]) {
            if (isVisited.find(ed) != isVisited.end()) continue;
            fi = nearestNeighbors.end();
            fi--;
            isVisited.insert(ed);
            td = q.dist(items[ed], key, size, bound);
            if ((td < fi->first) || nearestNeighbors.size() < ef) {
                candidates.insert(make_pair(td, ed));
                nearestNeighbors.insert(make_pair(td, ed));
                if (nearestNeighbors.size() > ef) nearestNeighbors.erase(fi);
            }
        }
    }
    vector<int> results;
    results.reserve(nearestNeighbors.size());
    for (auto &p: nearestNeighbors) results.push_back(p.second);
    return results;
}
