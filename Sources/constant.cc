#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "constant.h"

int ConstantSource::Data(int p) const
{
    (void)p;
    return value;
}

char *ConstantSource::TypeName() const
{
    return "ConstantSource";
}

ConstantSource::~ConstantSource()
{
}

