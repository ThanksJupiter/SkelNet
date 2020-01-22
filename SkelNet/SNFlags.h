#pragma once
void SetFlag(int &flags, char flagIndex)
{
	flags |= 1 << flagIndex;
}

void SetFlags(int &flags, int flagsToSet)
{
	flags |= flagsToSet;
}

void UnsetFlag(int &flags, char flagIndex)
{
	flags &= ~(1 << flagIndex);
}

void UnsetFlags(int &flags, int flagsToUnset)
{
	flags &= ~flagsToUnset;
}

bool GetFlag(int &flags, char flagIndex)
{
	return flags & (1 << flagIndex);
}

int GetFlags(int &flags, int flagsToGet)
{
	return flags & flagsToGet;
}

void ToggleFlag(int &flags, char flagIndex)
{
	flags ^= 1 << flagIndex;
}