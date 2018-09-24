#include "stdafx.h"
#include "Font.h"


Font::Font()
{
}
Character GetCharacter(int ASCII)
{

	return characters_Arial[ASCII - 32];


}

Font::~Font()
{
}
