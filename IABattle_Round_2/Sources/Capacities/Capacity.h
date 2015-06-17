#ifndef CAPACITY_H
#define CAPACITY_H

class Capacity
{
    public:
        Capacity(int);
        void upgrade();
        void downgrade();
        void virtual computeValue() = 0;
        //Getters and setters
        float getValue() const { return m_value; }
        unsigned int getLevel() const { return m_level; }
        void setValue(float v) { this->m_value = v; }
    protected:
        float m_value;
        unsigned int m_level;
};

#endif // CAPACITY_H
