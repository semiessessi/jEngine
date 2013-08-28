/* inTextReciever class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __INTEXTRECIEVER_H
#define __INTEXTRECIEVER_H

class inTextReciever
{
protected:
	wchar_t			inLine[1024];
	int				caretPos;
	bool			insertMode;
public:
	inTextReciever();
	virtual ~inTextReciever();

	void defCharHandler(unsigned int wParam, long lParam);
	void defKeyHandler(unsigned int wParam, long lParam);
	virtual void wmCharHandler(unsigned int wParam, long lParam);
	virtual void wmKeyHandler(unsigned int wParam, long lParam);
};

#endif