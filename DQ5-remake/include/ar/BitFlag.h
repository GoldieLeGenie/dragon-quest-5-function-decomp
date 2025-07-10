#pragma once
#include <type_traits>  // pour std::is_integral

namespace ar {

    template<typename T>
    class BitFlag {
        static_assert(std::is_integral<T>::value, "BitFlag requires an integral type");

    private:
        T flag_;

    public:
        BitFlag() : flag_(0) {}
        explicit BitFlag(T value) : flag_(value) {}

        // Vérifie si le bit 'bitIndex' est activé
        bool test(int bitIndex) const {
            return (flag_ >> bitIndex) & 1;
        }

        // Active le bit 'bitIndex'
        void set(int bitIndex) {
            flag_ |= (T(1) << bitIndex);
        }

        // Désactive le bit 'bitIndex'
        void clear(int bitIndex) {
            flag_ &= ~(T(1) << bitIndex);
        }

        // Renvoie la valeur brute du flag
        T get() const {
            return flag_;
        }

        // Réinitialise complètement le flag
        void reset() {
            flag_ = 0;
        }

        // Affecte une nouvelle valeur complète
        void setValue(T value) {
            flag_ = value;
        }
    };

    // Aliases
    using Flag32 = BitFlag<uint32_t>;
    using Flag8 = BitFlag<uint8_t>;

}  // namespace ar
