#pragma once

class Texture
{
public:
    Texture() = default;
    explicit Texture(const char* path, bool flip = true);

    void Bind(unsigned int slot = 0) const;

public:
    unsigned int ID;
};