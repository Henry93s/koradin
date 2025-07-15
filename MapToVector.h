#ifndef MAPTOVECTOR_H
#define MAPTOVECTOR_H

#include <vector>
#include <QMap>
#include <QString>

template <typename T>
static std::vector<T> MapToVector(const QMap<QString, T>& originalMap)
{
    std::vector<T> result;
    result.reserve(originalMap.size());

    for (auto it = originalMap.begin(); it != originalMap.end(); ++it) {
        result.push_back(it.value());
    }
    return result;
}

#endif // MAPTOVECTOR_H
