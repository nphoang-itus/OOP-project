#ifndef SEAT_CLASS_MAP_H
#define SEAT_CLASS_MAP_H

#include <string>
#include <map>
#include <sstream>
#include "SeatClass.h"
#include "../../exceptions/Result.h"

// Add comparison operator for SeatClass
inline bool operator<(const SeatClass &lhs, const SeatClass &rhs)
{
    return lhs.getCode() < rhs.getCode();
}

class SeatClassMap
{
private:
    std::map<SeatClass, int> _seatCounts;

public:
    // Make default constructor public
    SeatClassMap() = default;

    // Add comparison operators
    bool operator==(const SeatClassMap &other) const
    {
        return _seatCounts == other._seatCounts;
    }

    bool operator!=(const SeatClassMap &other) const
    {
        return !(*this == other);
    }

    static Result<SeatClassMap> create(const std::map<SeatClass, int> &seatCounts)
    {
        SeatClassMap map;
        map._seatCounts = seatCounts;
        return Success(map);
    }

    static Result<SeatClassMap> create(const std::string &seatLayoutStr)
    {
        SeatClassMap map;
        if (seatLayoutStr.empty())
        {
            return Success(map);
        }

        // Parse string format: "E:100,B:20,F:10"
        std::stringstream ss(seatLayoutStr);
        std::string item;
        while (std::getline(ss, item, ','))
        {
            size_t pos = item.find(':');
            if (pos == std::string::npos)
                continue;

            std::string code = item.substr(0, pos);
            int count = std::stoi(item.substr(pos + 1));

            auto seatClassResult = SeatClassRegistry::getByCode(code);
            if (!seatClassResult)
                continue;

            map._seatCounts[seatClassResult.value()] = count;
        }
        return Success(map);
    }

    bool hasSeatClass(const std::string &seatClassCode) const
    {
        auto seatClassResult = SeatClassRegistry::getByCode(seatClassCode);
        if (!seatClassResult)
            return false;
        return _seatCounts.find(seatClassResult.value()) != _seatCounts.end();
    }

    int getSeatCount(const std::string &seatClassCode) const
    {
        auto seatClassResult = SeatClassRegistry::getByCode(seatClassCode);
        if (!seatClassResult)
            return 0;
        auto it = _seatCounts.find(seatClassResult.value());
        return it != _seatCounts.end() ? it->second : 0;
    }

    bool isValidSeatNumber(const std::string &seatNumber) const
    {
        if (seatNumber.length() < 2)
            return false;
        std::string code = seatNumber.substr(0, 1);
        std::string number = seatNumber.substr(1);
        return hasSeatClass(code) && std::stoi(number) <= getSeatCount(code);
    }

    std::string toString() const
    {
        std::stringstream ss;
        for (const auto &[seatClass, count] : _seatCounts)
        {
            if (!ss.str().empty())
                ss << ",";
            ss << seatClass.getName() << ":" << count;
        }
        return ss.str();
    }

    const std::map<SeatClass, int> &getSeatCounts() const { return _seatCounts; }
};

#endif
