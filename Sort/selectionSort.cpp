/**The SELECTION-SORT algorithm.
*@param tab A pointer to the table to sort
*@param length The length of the table
*@param isIncreasing A flag value which indicate the order,
            no-0 value means to sort in the increasing order.
*@note The data will be sorted in the same table
*/
template <typename T>
void sort_selection(T *tab, int length, bool isIncreasing)
{
    if(length>1)
    {
        T key;//The element selected during each loop.
        int iKey;//The position of the current selected element
        int iLoop;//The index of the element to update
        int iLoop2;

        //The last element is obviously sorted already
        for(iLoop = 0; iLoop < length-1; iLoop++)
        {

            key = tab[iLoop];
            iKey = iLoop;
            for(iLoop2 = iLoop+1; iLoop2<length; iLoop2++)
            {
                if (isIncreasing && tab[iLoop2]<key ||
                    !isIncreasing && tab[iLoop2]>key)
                {
                	key = tab[iLoop2];
                    iKey = iLoop2;
                }
            }
            tab[iKey] = tab[iLoop];
            tab[iLoop] = key;
        }
    }
}
