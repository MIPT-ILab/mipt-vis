/**
 * File: UnitTest/main.cpp - Implementation and entry point for unit testing of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"


int main(int argc, char **argv)
{
	//Test graph package
    if ( !UTestGraph())
        return -1;

    if ( !UTestReadGraph( ))
		return -1;

    return 0;	
}