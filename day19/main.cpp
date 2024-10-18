#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>
#include <unordered_map>
#include <ranges>

constexpr auto maxULL = std::numeric_limits<unsigned long long>::max();

struct Part {
    unsigned long long int x;
    unsigned long long int m;
    unsigned long long int a;
    unsigned long long int s;
};

struct IntervalPart {
    std::pair<unsigned long long, unsigned long long> x{0, maxULL};
    std::pair<unsigned long long, unsigned long long> m{0, maxULL};
    std::pair<unsigned long long, unsigned long long> a{0, maxULL};
    std::pair<unsigned long long, unsigned long long> s{0, maxULL};
};

enum class Property {
    x = 0,
    m = 1,
    a = 2,
    s = 3
};

struct ConditionalRule {
    Property p;
    std::pair<unsigned long long, unsigned long long> interval;
    std::string dest;
};

struct UnconditionalRule {
    std::string dest;
};

using Rule = std::variant<ConditionalRule, UnconditionalRule>;

int main()
{
    auto f = std::ifstream { "example.txt" };

    std::vector<Part> parts;
    std::unordered_map<std::string, std::vector<Rule>> workflows;
    std::string line;
    while (std::getline(f, line)) {
        if (line.at(0) != '{') {
            // Workflows
            auto i = 0;
            while (line.at(i) != '{') i++;
            const auto name = line.substr(0, i);
            i++;

            auto split = line 
                | std::views::drop_while([](const auto c){ return c != '{'; }) 
                | std::views::drop(1)
                | std::views::take_while([](const auto c){ return c != '}'; })
                | std::views::split(',')
                | std::views::transform([](const auto&& s){return std::string_view(&*s.begin(), std::ranges::distance(s));})
            ;
            for (auto const r : split) {
                std::cout << r << '\n';
            }
            std::cout << '\n';

        } else {
            // Parts
            std::vector<unsigned long long> properties {};

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '=') {
                    properties.emplace_back(std::stoll(line.substr(i + 1)));
                }
            }

            parts.emplace_back(
                properties.at(0),
                properties.at(1),
                properties.at(2),
                properties.at(3));
        }
    }

    return 0;
}
