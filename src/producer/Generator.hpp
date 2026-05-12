#ifndef GENERATOR_HPP
#define GENERATOR_HPP

class Generator {
public:
    Generator() = default;
    virtual ~Generator() = default;
    
    virtual void next()=0;
};

class RampingGenerator : public Generator {
public:
    RampingGenerator(int minimum, int maximum, int step, int initial);
    ~RampingGenerator() override = default;
    
    void next() override;
    int getValue() const { return value; }

    int advance();

private:
    int minimum;
    int maximum;
    int step;
    int value;
};

#endif // GENERATOR_HPP