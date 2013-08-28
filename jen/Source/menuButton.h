/* menuButton class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUBUTTON_H
#define __MENUBUTTON_H

#include "menuItem.h"

class menuButton : public menuItem
{
private:
public:
	menuButton(menuSystem* ms, const wchar_t* code);
	~menuButton();

	void eventOnInit();
	void eventOnMouseDown();
	void eventOnMouseUp();
	void eventOnMouseExit();
};

#endif