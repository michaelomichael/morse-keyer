#pragma once

class KeyboardLayout {
   public:
    virtual char remap(char original) const = 0;
};

class DefaultKeyboardLayout : public KeyboardLayout {
   public:
    inline virtual char remap(char original) const override { return original; }
};

class DvorakKeyboardLayout : public KeyboardLayout {
   public:
    virtual char remap(char original) const override;
};
