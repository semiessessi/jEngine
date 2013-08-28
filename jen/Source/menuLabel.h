/* menuLabel class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENULABEL_H
#define __MENULABEL_H

#include "menuItem.h"

class menuLabel : public menuItem
{
private:
public:
	menuLabel(menuSystem* ms, const wchar_t* code);
	~menuLabel();

	void eventOnInit();
};

#endif