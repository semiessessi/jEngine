/* menuHScroll class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUHSCROLL_H
#define __MENUHSCROLL_H

#include "menuItem.h"

#include "menuButton.h"

class menuHScroll : public menuItem
{
private:
    class menuHSLeft : public menuButton
    {
    public:
        menuHSLeft(menuSystem* ms);

        void eventOnMouseDown();
        void eventOnMouseUp();
        void eventOnClick();
    };
    
    class menuHSRight : public menuButton
    {
    public:
        menuHSRight(menuSystem* ms);

        void eventOnMouseDown();
        void eventOnMouseUp();
        void eventOnClick();
    };

    class menuHSSlider : public menuButton
    {
    public:
        menuHSSlider(menuSystem* ms);
        void eventOnMouseDown();
        void eventOnMouseUp();
    };

    menuHSLeft*     l;
    menuHSRight*    r;
    menuHSSlider*   s;
public:
	menuHScroll(menuSystem* ms, const wchar_t* code);
	~menuHScroll();

	void eventOnInit();
	void eventOnMouseDown();
	void eventOnMouseUp();
	void eventOnMouseExit();
};

#endif