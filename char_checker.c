int char_checker(char *process)
{
    int i;
    i = 0;
    while (process[i] != '\0')
    {
        if (process[i] < '0' || process[i] > '9')
        {
            return(0);
        }
        i++;
    }
    return(1);
}