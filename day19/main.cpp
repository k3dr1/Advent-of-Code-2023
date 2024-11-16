#include <charconv>
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

unsigned long long partSum(const Part &part) {
    return (part.x + part.m + part.a + part.s);
}

unsigned long long consecutiveSum(const unsigned long long a, const unsigned long long b) {
    if (a > b) return consecutiveSum(b, a);
    return (b*(b+1)/2 - (a-1)*a/2);
}

struct IntervalPart {
    std::pair<unsigned long long, unsigned long long> x{1ULL, maxULL};
    std::pair<unsigned long long, unsigned long long> m{1ULL, maxULL};
    std::pair<unsigned long long, unsigned long long> a{1ULL, maxULL};
    std::pair<unsigned long long, unsigned long long> s{1ULL, maxULL};
};

enum class Property {
    x = 'x',
    m = 'm',
    a = 'a',
    s = 's'
};

struct ConditionalRule {
    Property property;
    std::pair<unsigned long long, unsigned long long> interval;
    std::string dest;
};

struct UnconditionalRule {
    std::string dest;
};

using Rule = std::variant<ConditionalRule, UnconditionalRule>;

bool inInterval(const auto &x, const auto &interval) {
    return (interval.first <= x) && (x <= interval.second);
}

bool isConditional(Rule rule) {
    return rule.index() == 0;
}

std::pair<unsigned long long, unsigned long long> invertRuleInterval(std::pair<unsigned long long, unsigned long long> interval)
{
    if (interval.first == 1) {
        return { interval.second + 1, maxULL };
    } else {
        return { 1, interval.first - 1 };
    }
}

bool checkRule(Part part, Rule rule)
{
    switch (rule.index()) {
        case 0: {
            const auto cRule = std::get<ConditionalRule>(rule);
            switch (cRule.property) {
                case Property::x: return inInterval(part.x, cRule.interval);
                case Property::m: return inInterval(part.m, cRule.interval);
                case Property::a: return inInterval(part.a, cRule.interval);
                case Property::s: return inInterval(part.s, cRule.interval);
            }
        }
        case 1: {
            return true;
        }
        default: {
            std::cout << "checkRule default hit\n";
            return false;
        }
    }
}

auto& getProperty(IntervalPart &part, Property property) {
    switch (property) {
        case (Property::x): return part.x;
        case (Property::m): return part.m;
        case (Property::a): return part.a;
        case (Property::s): return part.s;
    }
}

std::pair<unsigned long long, unsigned long long> intervalIntersection(
    std::pair<unsigned long long, unsigned long long> a,
    std::pair<unsigned long long, unsigned long long> b)
{
    auto smallerTop = std::min(a.second, b.second);
    auto largerBot = std::max(a.first, b.first);
    if (largerBot <= smallerTop) {
        return {largerBot, smallerTop};
    } else {
        return {0,0};
    }
}

std::vector<IntervalPart> construct(
    IntervalPart intervalPart,
    const std::string &name,
    int ruleIdx,
    const std::unordered_map<std::string, std::vector<Rule>>& workflows
)
{
    if (name == "A") return {intervalPart};
    if (name == "R") return {};

    const auto workflow = workflows.at(name);
    const auto rule = workflow.at(ruleIdx);

    if (!isConditional(rule)) {
        const auto unconditionalRule = std::get<UnconditionalRule>(rule);
        return construct(intervalPart, unconditionalRule.dest, 0, workflows);
    }

    const auto conditionalRule = std::get<ConditionalRule>(rule);
    const auto ruleInterval = conditionalRule.interval;

    const auto acceptedInterval{intervalIntersection(
        getProperty(intervalPart, conditionalRule.property),
        conditionalRule.interval
    )};
    auto acceptedIntervalPart = IntervalPart{intervalPart};
    getProperty(acceptedIntervalPart, conditionalRule.property).first = acceptedInterval.first;
    getProperty(acceptedIntervalPart, conditionalRule.property).second = acceptedInterval.second;

    const auto rejectedInterval{intervalIntersection(
        getProperty(intervalPart, conditionalRule.property),
        invertRuleInterval(conditionalRule.interval)
    )};
    auto rejectedIntervalPart = IntervalPart{intervalPart};
    getProperty(rejectedIntervalPart, conditionalRule.property).first = rejectedInterval.first;
    getProperty(rejectedIntervalPart, conditionalRule.property).second = rejectedInterval.second;

    std::vector<IntervalPart> res{};
    if (acceptedInterval != std::make_pair(0ULL,0ULL)) {
        const auto acceptedResults = construct(acceptedIntervalPart,
            conditionalRule.dest,
            0,
            workflows);
        for (const auto acceptedResult : acceptedResults) {
            res.push_back(acceptedResult);
        }
    }
    if (rejectedInterval != std::make_pair(0ULL, 0ULL)) {
        const auto rejectedResults = construct(rejectedIntervalPart, name, ruleIdx+1, workflows);
        for (const auto rejectedResult : rejectedResults) {
            res.push_back(rejectedResult);
        }
    }

    return res;
}

bool trace(
    Part part,
    std::string &firstWorkflow,
    std::unordered_map<std::string, std::vector<Rule>> &workflows)
{
    auto workflow = workflows.at(firstWorkflow);
    auto i = 0;
    while (true) {
        auto rule = workflow.at(i);
        if (checkRule(part, rule)) {
            i = 0;
            const auto dest = std::visit([](const auto &r){ return r.dest; }, rule);
            if (dest == "A" || dest == "R") {
                return dest=="A";
            }
            else {

                //std::cout << "dest " << dest << '\n';

                workflow = workflows.at(dest);
            }
        } else {
            i++;
        }
    }
}

int main()
{
    auto f = std::ifstream { "input.txt" };

    std::vector<Part> parts;
    std::unordered_map<std::string, std::vector<Rule>> workflows;
    std::string line;
    while (std::getline(f, line)) {
        if (line.at(0) != '{') {
            // skip if empty line
            if (line.length() <= 1) continue;

            // Workflows
            std::vector<Rule> workflow;
            const auto name = line.substr(0, line.find('{'));
            std::cout << "Parsing workflow: " << name << '\n';

            auto split = line 
                | std::views::drop_while([](const auto c){ return c != '{'; }) 
                | std::views::drop(1)
                | std::views::take_while([](const auto c){ return c != '}'; })
                | std::views::split(',')
                | std::views::transform([](const auto&& s){return std::string_view(&*s.begin(), std::ranges::distance(s));});

            for (auto const &r : split) {
                if (r.contains(':')) {
                    // Conditional rule
                    const auto p = Property { r.at(0) };
                    auto num = 0ULL;
                    std::from_chars(r.data() + 2, r.data() + r.size(), num, 10);

                    workflow.emplace_back(
                        ConditionalRule(
                            p,
                            (r.at(1) == '<') ? std::make_pair(1ULL, num-1) : std::make_pair(num+1, maxULL),
                            std::string { r.substr(r.find(':')+1) }));

                    auto back = std::get<ConditionalRule>(workflow.at(workflow.size() - 1));

                } else {
                    // Unconditional rule

                    workflow.emplace_back(UnconditionalRule{std::string(r)});
                }
            }
            std::cout << "Adding the workflow: " << name << "\n\n";
            workflows[name] = workflow;
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

    auto part1 = 0ULL;
    auto part2 = 0ULL;
    auto count = 0;
    std::string start = "in";

    for (const auto &part : parts) {
        count++;
        auto status = trace(part, start, workflows);
        if (status) part1 += partSum(part);
    }

    std::cout << "Answer for part 1 is " << part1 << '\n';

    auto intervalPart = IntervalPart(
        {1ULL, 4000ULL},
        {1ULL, 4000ULL},
        {1ULL, 4000ULL},
        {1ULL, 4000ULL}
    );

    const auto generatedParts = 
        construct(intervalPart, "in", 0, workflows);

    for (const auto part : generatedParts) {
        part2 += (part.x.second - part.x.first + 1)
               * (part.m.second - part.m.first + 1)
               * (part.a.second - part.a.first + 1)
               * (part.s.second - part.s.first + 1);
    }

    std::cout << "Answer for part 2 is " << part2 << '\n';

    return 0;
}
