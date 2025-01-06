#include "number.h"

int GetSize(const int2023_t &number) {
    int size;
    for (size = 251; size >= 0; --size) {
        if (number.arr[size]) {
            break;
        }
    }

    return size + 1;
}

int2023_t from_int(int32_t i) {
    int2023_t number;
    if (i < 0) {
        number.sign = false;
    }
    uint64_t mod = abs(i);
    int j = 0;
    while (mod > 0) {
        number.arr[j] = mod % 256;
        ++j;
        mod /= 256;
    }

    return number;
}

int2023_t from_string(const char* buff) {
    int2023_t number;
    int index = 0;
    if (buff[index] == '-') {
        ++index;
    }
    const int2023_t ten = from_int(10);
    while (index < strlen(buff)) {
        number = number * ten + from_int(static_cast<int>(buff[index]) - 48);
        ++index;
    }
    if (buff[0] == '-') {
        number.sign = false;
    }

    return number;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t number;

    if (lhs.sign == rhs.sign) {
        number.sign = lhs.sign;
        uint16_t writing_number = 0;
        for (int i = 0; i < std::max(GetSize(lhs), GetSize(rhs)) + 1; ++i) {
            writing_number += lhs.arr[i] + rhs.arr[i];
            number.arr[i] = writing_number % 256;
            writing_number /= 256;
        }

    } else {
        int2023_t value;
        for (int i = 0; i < GetSize(rhs); ++i) {
            value.arr[i] = rhs.arr[i];
        }
        value.sign = lhs.sign;

        number = lhs - value;
    }

    return number;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t number;

    if (lhs.sign && !rhs.sign) {
        int2023_t value;
        for (int i = 0; i < GetSize(rhs); ++i) {
            value.arr[i] = rhs.arr[i];
        }

        number = lhs + value;
    } else if (!lhs.sign && rhs.sign) {
        int2023_t value;
        value.sign = false;
        for (int i = 0; i < GetSize(rhs); ++i) {
            value.arr[i] = rhs.arr[i];
        }

        number = lhs + value;
    } else if (lhs == rhs) {

        number = from_int(0);
    } else {
        if (rhs > lhs) {
            number.sign = false;
        }

        if (lhs.sign && lhs > rhs || !lhs.sign && rhs > lhs) {
            int16_t writing_number;
            uint8_t digit = 0;
            for (int i = 0; i < GetSize(lhs); ++i) {
                writing_number = static_cast<int16_t>(lhs.arr[i] - digit - rhs.arr[i]);
                if (writing_number < 0) {
                    writing_number += 256;
                    digit = 1;
                } else {
                    digit = 0;
                }
                number.arr[i] = static_cast<uint8_t>(writing_number);
            }
        } else {
            int16_t writing_number;
            uint8_t digit = 0;
            for (int i = 0; i < GetSize(rhs); ++i) {
                writing_number = static_cast<int16_t>(rhs.arr[i] - digit - lhs.arr[i]);
                if (writing_number < 0) {
                    writing_number += 256;
                    digit = 1;
                } else {
                    digit = 0;
                }
                number.arr[i] = static_cast<uint8_t>(writing_number);
            }
        }
    }


    return number;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    if (lhs == from_int(0) || rhs == from_int(0)) {

        return from_int(0);
    }

    int2023_t number;
    if (lhs.sign != rhs.sign) {
        number.sign = false;
    }

    for (int i = 0; i < GetSize(rhs); ++i) {
        uint32_t writing_number = 0;
        for (int j = 0; j < GetSize(lhs) + 1; ++j) {
            writing_number += lhs.arr[j] * rhs.arr[i];
            writing_number += number.arr[i + j];
            number.arr[i + j] = writing_number % 256;
            writing_number /= 256;
        }
    }

    return number;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {

    int2023_t lhs_mod = lhs;
    lhs_mod.sign = true;
    int2023_t rhs_mod = rhs;
    rhs_mod.sign = true;

    if (rhs_mod > lhs_mod) {
        return from_int(0);
    }

    int2023_t number;
    const int2023_t ten = from_int(10);
    int2023_t divisor = from_int(1);
    while (lhs_mod > rhs_mod || lhs_mod == rhs_mod) {
        while (lhs_mod > rhs_mod * divisor * ten || lhs_mod == rhs_mod * divisor * ten) {
            divisor = divisor * ten;
        }
        lhs_mod = lhs_mod - rhs_mod * divisor;
        number = number + divisor;
        divisor = from_int(1);
    }
    if (lhs.sign != rhs.sign) {
        number.sign = false;
    }

    return number;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    if (GetSize(lhs) != GetSize(rhs) || lhs.sign !=rhs.sign) {

        return false;
    }
    for (int i = 0; i < GetSize(lhs); ++i) {
        if (lhs.arr[i] != rhs.arr[i]) {

            return false;
        }
    }

    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    if (GetSize(lhs) != GetSize(rhs) || lhs.sign !=rhs.sign) {

        return true;
    }
    for (int i = 0; i < GetSize(lhs); ++i) {
        if (lhs.arr[i] != rhs.arr[i]) {

            return true;
        }
    }

    return false;
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    if (lhs == rhs) {
        return false;
    }
    if (!lhs.sign && rhs.sign) {
        return false;
    }
    if (lhs.sign && rhs.sign) {
        if (GetSize(lhs) < GetSize(rhs)) {
            return false;
        }
        if (GetSize(lhs) == GetSize(rhs)) {
            int index = GetSize(lhs) - 1;
            while (index >= 0) {
                if (lhs.arr[index] == rhs.arr[index]) {
                    --index;
                } else if (lhs.arr[index] > rhs.arr[index]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    if (!lhs.sign && !rhs.sign) {
        if (GetSize(lhs) > GetSize(rhs)) {
            return false;
        }
        if (GetSize(lhs) == GetSize(rhs)) {
            int index = GetSize(lhs) - 1;
            while (index >= 0) {
                if (lhs.arr[index] == rhs.arr[index]) {
                    --index;
                } else if (lhs.arr[index] < rhs.arr[index]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    if (!value.sign) {
        stream << '-';
    }
    for (int i = GetSize(value) - 1; i >= 0; --i) {
        stream << static_cast<uint16_t>(value.arr[i]);
    }

    return stream;
}
