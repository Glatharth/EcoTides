#ifndef ECOTIDES_CHOICE_HPP
#define ECOTIDES_CHOICE_HPP

enum class Decision {
    No,
    Yes
};

class Choice {
public:
    static constexpr int smallChange = 1;
    static constexpr int largeChange = 2;

    static int computeChange(Decision decision, int baseValue, bool isLargeChange);
};

#endif
