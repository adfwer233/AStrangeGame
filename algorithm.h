#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "statusConstant.h"
#include <vector>

class Algorithm {
public:
    Algorithm();

    static void findPathBFS(const std::vector<std::vector<coordinateStatus>>& t_map, std::vector<std::vector<actionStatus>>& actionResult,
                            std::vector<std::vector<std::pair<int, int>>>& t_prev, int maxLength);
};

#endif  // ALGORITHM_H
