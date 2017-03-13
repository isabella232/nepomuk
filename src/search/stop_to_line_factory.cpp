#include "search/stop_to_line_factory.hpp"
#include "timetable/line_table.hpp"

#include <algorithm>

namespace transit
{
namespace search
{

StopToLine StopToLineFactory::produce(std::size_t const num_stops,
                                      timetable::TimeTable const &timetable)
{
    StopToLine look_up;

    look_up.trip_table.resize(num_stops);
    for (std::size_t line_id = 0; line_id < timetable.line_tables.size(); ++line_id)
    {
        // get stops of the line
        auto const &stop_table = timetable.line_tables[line_id].serviced_stops;
        auto const add_to_lookup = [&look_up, line_id](auto const &stop_arrival) {
            look_up.trip_table[static_cast<std::uint64_t>(stop_arrival.stop_id)].push_back(
                timetable::LineID{line_id});
        };
        std::for_each(
            stop_table.stop_arrivals.begin(), stop_table.stop_arrivals.end(), add_to_lookup);
    }

    return look_up;
}

} // namespace search
} // namespace transit
