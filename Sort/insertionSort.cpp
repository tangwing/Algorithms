/**The INSERTION-SORT algorithm.
*@param tab A pointer to the table to sort
*@param length The length of the table
*@param isIncreasing A flag value which indicate the order,
            no-0 value means to sort in the increasing order.
*@note The data will be sorted in the same table
*/
template <typename T>
void sort_insertion(T *tab, int length, bool isIncreasing)
{
    if(length>1)
    {
        T key;//The element to be inserted.
        int iKey;//The index of the element to be inserted
        int iPos;//The index of the position to insert an element
        //The first element is obviously sorted already
        for(iKey = 1; iKey < length; iKey++)
        {

            key = tab[iKey];
            iPos = iKey-1;
            while(iPos >= 0 )
            {
                if (isIncreasing && tab[iPos] <= key)
                	break;
                else if (!isIncreasing && tab[iPos]>key)
                	break;
                else
                    tab[iPos+1] = tab[iPos];
                iPos--;
            }
            tab[iPos+1] = key;
        }
    }
}
