/* jShared library by Semi Essessi
 *
 * Some system global things for jEngine
 *
 */

#define dlli __declspec(dllimport)

dlli void jsCatchKeys(bool enable);
dlli bool jsHasInstanced();
dlli void jsInstance();
dlli void jsDeInstance();