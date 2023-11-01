#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <valarray>
#include <windows.h>

#include "Classes_declarations.hpp"

int main();

/////////////////////
// MATRIX DEFINITIONS
/////////////////////

Matrix::Matrix(std::string id, int dim, int ydim)
{

    matrix_id = id;
    associated_dimension = dim;
    associated_y_dimension = ydim;
    aug_xdim = 0;
    aug_ydim = 0;
    p_head = nullptr;
    aug_p_head = nullptr;
    is_augmented = false;
    next_matrix = nullptr;
}

Matrix::~Matrix()
{
    // empty
}

Matrix *Matrix::get_next_matrix() { return next_matrix; }

void Matrix::set_next_matrix(Matrix *p_new_matrix)
{
    next_matrix = p_new_matrix;
    return;
}

Entry *Matrix::get_head() { return p_head; }

void Matrix::set_head(Entry *p_new_head)
{
    p_head = p_new_head;
    return;
}

std::string Matrix::get_id() { return matrix_id; }

void Matrix::set_id(std::string new_id)
{
    matrix_id = new_id;
    return;
}

int Matrix::get_x_dimension() { return associated_dimension; }

int Matrix::get_y_dimension() { return associated_y_dimension; }

void Matrix::print_steps_true()
{
    print_steps = true;
    return;
}

void Matrix::print_steps_false()
{
    print_steps = false;
    return;
}

double Matrix::retrieve_value_from_entry(int x, int y)
{

    double var{0.0};

    Entry *temp;
    temp = p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    var = temp->get_value();

    return var;
}

void Matrix::print_Matrix()
{

    double temp_print;
    int biggest_row{0};
    int aug_biggest_row{0};
    int count{1};

    if (is_augmented == false)
    {

        Entry *temp;
        temp = p_head;

        int row_size_array[associated_y_dimension];

        for (int k{1}; k <= associated_y_dimension; k++)
        {

            row_size_array[k - 1] = size_of_row(k);
        }

        // This are to try and correctly print everything in line
        biggest_row = largest_row() + associated_dimension + 1; // total characters + number of spaces + the first space at the start of the line

        std::cout << "Matrix ID: " << matrix_id << std::endl;
        std::cout << "---";
        for (int k{0}; k <= (biggest_row - 2); k++)
        {
            std::cout << " ";
        }
        std::cout << "---" << std::endl;

        std::cout << "|  ";

        int y{0};
        while (temp != nullptr)
        {

            if (count > associated_dimension)
            { // End of the current row of the matrix

                row_size_array[y] = row_size_array[y] + associated_dimension; // ignore +1 because the if condition is "strictly larger than"
                for (int k{1}; k <= (biggest_row - row_size_array[y]); k++)
                {
                    std::cout << " ";
                }
                y++;

                std::cout << "|";
                std::cout << std::endl;
                std::cout << "|  ";

                count = 1;
            }

            temp_print = temp->get_value();

            convert_to_fraction(temp_print);
            std::cout << "  ";
            count++;

            temp = temp->get_next();
        }

        row_size_array[y] = row_size_array[y] + associated_dimension; // ignore +1 because the if condition is "strictly larger than"
        for (int k{1}; k <= (biggest_row - row_size_array[y]); k++)
        {
            std::cout << " ";
        }
        std::cout << "|" << std::endl;

        std::cout << "---";
        for (int k{0}; k <= (biggest_row - 2); k++)
        {
            std::cout << " ";
        }
        std::cout << "---" << std::endl;
    }
    else
    {

        Entry *augmented_temp;
        augmented_temp = aug_p_head;

        double columns_num;

        columns_num = aug_xdim; // columns_num is an old variable. didnt want to change the names of everything

        int row_size_array[associated_y_dimension];

        for (int k{1}; k <= associated_y_dimension; k++)
        {

            row_size_array[k - 1] = size_of_row(k);
        }

        int aug_row_size_array[aug_ydim];

        for (int k{1}; k <= associated_y_dimension; k++)
        {

            aug_row_size_array[k - 1] = aug_size_of_row(k);
        }

        // These are to try and correctly print everything in line
        biggest_row = largest_row() + associated_dimension + 1; // see non augmented version of this function (above) for details on calculations
        aug_biggest_row = aug_largest_row() + aug_xdim + 1;

        std::cout << "Matrix ID: " << matrix_id << " [AUGMENTED]" << std::endl;
        std::cout << "---";
        for (int k{1}; k <= (biggest_row + aug_biggest_row - 2); k++)
        {
            std::cout << " ";
        }
        std::cout << "---" << std::endl;
        std::cout << "| ";

        Entry *temp;
        temp = p_head;

        augmented_temp = aug_p_head;

        int y{0};
        while (augmented_temp != nullptr)
        {

            if (count > associated_dimension)
            { // If we have printed an entire row of entries (regular and augmented), go to the next row

                row_size_array[y] = row_size_array[y] + associated_dimension; // ignore +1 because the if condition is "strictly larger than"
                for (int k{1}; k <= (biggest_row - row_size_array[y]); k++)
                {
                    std::cout << " ";
                }

                std::cout << "| ";
                for (int k{1}; k <= columns_num; k++)
                {
                    convert_to_fraction(augmented_temp->get_value());
                    std::cout << " ";
                    augmented_temp = augmented_temp->get_next();
                }

                aug_row_size_array[y] = aug_row_size_array[y] + aug_xdim;
                for (int k{1}; k <= (aug_biggest_row - aug_row_size_array[y]); k++)
                {
                    std::cout << " ";
                }

                y++;

                std::cout << "|";
                if (temp == nullptr)
                {
                    break;
                }
                std::cout << std::endl;
                std::cout << "| ";

                count = 1;
            }

            if (temp == nullptr)
            {
                break;
            }

            temp_print = temp->get_value();

            convert_to_fraction(temp->get_value());
            std::cout << " ";

            temp = temp->get_next();
            count++;
        }

        std::cout << std::endl;
        std::cout << "---";
        for (int k{1}; k <= (biggest_row + aug_biggest_row - 2); k++)
        {
            std::cout << " ";
        }
        std::cout << "---" << std::endl;
    }

    return;
}

int Matrix::size_of_row(int y)
{

    std::string temp_str;

    int longest{0};
    int current{0};
    int current_row{0};

    for (int x{1}; x <= associated_dimension; x++)
    {

        current = std::to_string(retrieve_value_from_entry(x, y)).length();
        temp_str = std::to_string(retrieve_value_from_entry(x, y));

        for (int k{0}; k <= temp_str.length(); k++)
        {

            // IF this char equals 0 AND the next char is 0 OR null, count--              OR IF this char is "." and the next char is 0, count--
            // example: temp_str is 5.000000, so at temp_str[1], it is a ".", and the next character (temp_str[2]) is a 0, therefore count--
            // This is done because otherwise the .length() function will count those 6 extras zeroes when we dont want it to
            if ((temp_str[k] == '0' && (temp_str[k + 1] == '0' || temp_str[k + 1] == '\0')) || (temp_str[k] == '.' && temp_str[k + 1] == '0'))
            {
                current--;
            }
        }

        current_row = current_row + current;
    }

    return current_row;
}

int Matrix::aug_size_of_row(int y)
{

    std::string temp_str;

    int longest{0};
    int current{0};
    int current_row{0};

    for (int x{1}; x < associated_dimension; x++)
    { // was x <= associated_dimension BEFORE

        current = std::to_string(aug_retrieve_value_from_entry(x, y)).length();
        temp_str = std::to_string(aug_retrieve_value_from_entry(x, y));

        for (int k{0}; k <= temp_str.length(); k++)
        {

            // IF this char equals 0 AND the next char is 0 OR null, count--              OR IF this char is "." and the next char is 0, count--
            // example: temp_str is 5.000000, so at temp_str[1], it is a ".", and the next character (temp_str[2]) is a 0, therefore count--
            // This is done because otherwise the .length() function will count those 6 extras zeroes when we dont want it to
            if ((temp_str[k] == '0' && (temp_str[k + 1] == '0' || temp_str[k + 1] == '\0')) || (temp_str[k] == '.' && temp_str[k + 1] == '0'))
            {
                current--;
            }
        }

        current_row = current_row + current;
    }

    return current_row;
}

int Matrix::largest_row()
{

    std::string temp_str;

    int longest{0};
    int current{0};
    int current_row{0};

    for (int i{1}; i <= associated_y_dimension; i++)
    { // y

        for (int j{1}; j <= associated_dimension; j++)
        { // x

            current = return_fraction_conversion(retrieve_value_from_entry(j, i)).length();
            temp_str = return_fraction_conversion(retrieve_value_from_entry(j, i));

            for (int k{0}; k <= temp_str.length(); k++)
            {

                // IF this char equals 0 AND the next char is 0 OR null, count--              OR IF this char is "." and the next char is 0, count--
                // example: temp_str is 5.000000, so at temp_str[1], it is a ".", and the next character (temp_str[2]) is a 0, therefore count--
                // This is done because otherwise the .length() function will count those 6 extras zeroes when we dont want it to
                if ((temp_str[k] == '0' && (temp_str[k + 1] == '0' || temp_str[k + 1] == '\0')) || (temp_str[k] == '.' && temp_str[k + 1] == '0'))
                {
                    current--;
                }
            }

            current_row = current_row + current;
        }

        if (current_row > longest)
        {
            longest = current_row;
        }

        current = 0;
        current_row = 0;
    }

    return longest;
}

int Matrix::aug_largest_row()
{

    std::string temp_str;

    int longest{0};
    int current{0};
    int current_row{0};

    for (int i{1}; i <= aug_ydim; i++)
    { // y

        for (int j{1}; j <= aug_xdim; j++)
        { // x

            current = return_fraction_conversion(aug_retrieve_value_from_entry(j, i)).length(); // std::to_string
            temp_str = return_fraction_conversion(aug_retrieve_value_from_entry(j, i));         // std::to_string

            for (int k{0}; k <= temp_str.length(); k++)
            {

                // IF this char equals 0 AND the next char is 0 OR null, count--              OR IF this char is "." and the next char is 0, count--
                // example: temp_str is 5.000000, so at temp_str[1], it is a ".", and the next character (temp_str[2]) is a 0, therefore count--
                // This is done because otherwise the .length() function will count those 6 extras zeroes when we dont want it to
                if ((temp_str[k] == '0' && (temp_str[k + 1] == '0' || temp_str[k + 1] == '\0')) || (temp_str[k] == '.' && temp_str[k + 1] == '0'))
                {
                    current--;
                }
            }

            current_row = current_row + current;
        }

        if (current_row > longest)
        {
            longest = current_row;
        }

        current = 0;
        current_row = 0;
    }

    return longest;
}

void Matrix::insert_into_LL(Entry *p_new)
{

    Entry *prev_temp = p_head;
    Entry *temp = p_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next();
    }

    prev_temp->set_next(p_new);

    return;
}

void Matrix::create_matrix()
{

    double temp_var;
    int i{1};
    int j{1};

    for (int k{1}; k <= associated_y_dimension; k++)
    { // associated dimension

        for (int l{1}; l <= associated_dimension; l++)
        {

            std::cout << "Enter the value of the entry at (" << i << ", " << j << "): ";
            std::cin >> temp_var;

            Entry *p_temp{new Entry{i, j, temp_var}};

            if (k == 1 && l == 1)
            { // if this is the 1st entry, set it as the head

                p_head = p_temp;
            }
            else
            {

                Matrix::insert_into_LL(p_temp);
            }

            i++;
        }

        j++;
        i = 1;
    }

    return;
}

void Matrix::create_augmented_matrix()
{

    if (associated_dimension == associated_y_dimension)
    { // Offer to create identity matrix for user if they have a square matrix

        std::string choice_variable;

        std::cout << "Would you like the augmented matrix to be the identity matrix? [Y] or [N]: ";
        std::cin >> choice_variable;

        if (choice_variable == "Y" || choice_variable == "y")
        {
            create_identity_augmented_matrix();
            return;
        }
    }

    double temp_var;
    int i{1};
    int j{1};

    int xdim;
    int ydim{associated_y_dimension};

    std::cout << "Enter the x-size of the augmented matrix: ";
    std::cin >> xdim;
    set_augmented_xdim(xdim);

    // std::cout << "Enter the y-size of the augmented matrix: "; //DEBUG ONLY
    // std::cin >> ydim;
    set_augmented_ydim(ydim);

    for (int k{1}; k <= ydim; k++)
    {

        for (int l{1}; l <= xdim; l++)
        {

            std::cout << "Enter the value of the entry at (" << i << ", " << j << "): ";
            std::cin >> temp_var;

            Entry *p_aug_temp{new Entry{i, j, temp_var}};

            if (k == 1 && l == 1)
            {

                aug_p_head = p_aug_temp;
            }
            else
            {

                Matrix::augmented_insert_into_LL(p_aug_temp);
            }

            i++;
        }

        j++;
        i = 1;
    }

    is_augmented = true;

    return;
}

void Matrix::create_identity_augmented_matrix()
{

    set_augmented_xdim(associated_dimension);
    set_augmented_ydim(associated_dimension);

    double identity_array[associated_dimension * associated_dimension];
    int array_tracker{0};

    for (int k{1}; k <= associated_dimension; k++)
    { // Create the array to feed the identity matrix into the constructor

        for (int l{1}; l <= associated_dimension; l++)
        {

            if (k == l)
            {
                identity_array[array_tracker] = 1;
            }
            else
            {
                identity_array[array_tracker] = 0;
            }

            array_tracker++;
        }
    }

    int i{1};
    int j{1};
    array_tracker = 0;

    int xdim{associated_dimension};
    int ydim{associated_y_dimension};

    for (int k{1}; k <= ydim; k++)
    {

        for (int l{1}; l <= xdim; l++)
        {

            Entry *p_aug_temp{new Entry{i, j, identity_array[array_tracker]}}; // just creates an augmented matrix with a diagonal of 1's
            array_tracker++;

            if (k == 1 && l == 1)
            {

                aug_p_head = p_aug_temp;
            }
            else
            {

                Matrix::augmented_insert_into_LL(p_aug_temp);
            }

            i++;
        }

        j++;
        i = 1;
    }

    is_augmented = true;

    return;
}

void Matrix::set_augmented_xdim(int a_xdim)
{

    aug_xdim = a_xdim;

    return;
}

void Matrix::set_augmented_ydim(int a_ydim)
{

    aug_ydim = a_ydim;

    return;
}

void Matrix::augmented_insert_into_LL(Entry *p_new)
{

    Entry *prev_temp = aug_p_head;
    Entry *temp = aug_p_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next();
    }

    prev_temp->set_next(p_new);

    return;
}

void Matrix::edit_value(double value, int x, int y)
{

    Entry *temp;
    temp = p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    temp->internal_set_value(value);

    return;
}

double Matrix::aug_retrieve_value_from_entry(int x, int y)
{

    double var{0.0};

    Entry *temp;
    temp = aug_p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    var = temp->get_value();

    return var;
}

void Matrix::aug_edit_value(double value, int x, int y)
{

    Entry *temp;
    temp = aug_p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    temp->internal_set_value(value);

    return;
}

void Matrix::row_reduce()
{

    bool diagonal_is_ones;

    bool row_of_zeroes;

    int inconsistency_dummyvar;

    for (int m{1}; m <= associated_y_dimension; m++)
    { //'m' will represent a row, or a y coordinate

        //
        // Check to see if the matrix is in RREF
        // Only works in square matrices
        /*
        if ( associated_dimension == associated_y_dimension ) {

            for ( int k{1}; k <= associated_dimension; k++ ) {

                if ( retrieve_value_from_entry( k, k ) != 1 ) {

                    diagonal_is_ones = false;
                    break;
                } else {

                    diagonal_is_ones = true;
                }
            }

            if ( diagonal_is_ones ) {
                break;
            }
        }
        */
        //
        // END: Check to see if the matrix is in RREF
        //

        //
        // Row reducer engine
        //
        double dont_div_by_zero;
        dont_div_by_zero = retrieve_value_from_entry(m, m); // ensure that the pivot is not 0, to avoid a division by 0

        if (dont_div_by_zero != 0)
        {
            rr_row_scale(m, 1 / retrieve_value_from_entry(m, m)); // scale that row so the pivot will be equal to 1
        }

        /*
        if ( retrieve_value_from_entry( m, m ) == 1 ) {



        }
        */

        inconsistency_dummyvar = inconsistency_check();
        if (inconsistency_dummyvar == 1)
        {
            return;
        }

        if (print_steps == true)
        {
            print_Matrix();
        }

        //'n' is the variable that represents which row we are scaling to create
        // a column of zeroes for the pivot
        for (int n{1}; n <= associated_y_dimension; n++)
        {

            if (n == m)
            { // Skip the row with the current pivot
                continue;
            }

            //'m' is both an x and y coord, we are scaling
            // based on the numbers above and below the pivot
            rr_scale_then_add(n, m, retrieve_value_from_entry(m, n)); // scale this row so that a column of 0's will be created for the pivot

            inconsistency_dummyvar = inconsistency_check();
            if (inconsistency_dummyvar == 1)
            {
                return;
            }

            if (print_steps == true)
            {
                print_Matrix();
            }
        }
        //
        // END: Row reducer engine
        //
    }

    // Tragic band-aid fix. Unfortunately despite my best efforts with an infinite while loop, the current RR engine using
    // for loops will sometimes not make it all the way to a diagonal of 1's in cases where a row has elements that are
    // multiples of another row. So recursive calls to row_reduce() will force the diagonal of 1's at all costs.

    if (associated_dimension != associated_y_dimension)
    { // Band aid for non square matrices. Solely to skip the recursive call

        diagonal_is_ones = true;
    }
    else
    {

        for (int k{1}; k <= associated_dimension; k++)
        {

            if (retrieve_value_from_entry(k, k) != 1)
            {

                diagonal_is_ones = false;
                break;
            }
            else
            {

                diagonal_is_ones = true;
            }
        }
    }

    if (diagonal_is_ones == false)
    {
        row_reduce2(); // This function is the exact same as this one except it has no recursive call to row reduce. (prevent infinite loop)
    }

    remove_garbage();

    print_Matrix();

    return;
}

void Matrix::row_reduce2()
{

    bool diagonal_is_ones;

    bool row_of_zeroes;

    int inconsistency_dummyvar;

    for (int m{1}; m <= associated_y_dimension; m++)
    { //'m' will represent a row, or a y coordinate

        //
        // Check to see if the matrix is in RREF
        // Only works in square matrices
        if (associated_dimension == associated_y_dimension)
        {

            for (int k{1}; k <= associated_dimension; k++)
            {

                if (retrieve_value_from_entry(k, k) != 1)
                {

                    diagonal_is_ones = false;
                    break;
                }
                else
                {

                    diagonal_is_ones = true;
                }
            }

            if (diagonal_is_ones)
            {
                break;
            }
        }
        //
        // END: Check to see if the matrix is in RREF
        //

        //
        // Row reducer engine
        //
        int dont_div_by_zero;
        dont_div_by_zero = retrieve_value_from_entry(m, m); // Ensure that the entry in question is not 0, to avoid a division by 0

        if (dont_div_by_zero != 0)
        {
            rr_row_scale(m, 1 / retrieve_value_from_entry(m, m)); // Scale that entire row so the pivot is 1
        }

        inconsistency_dummyvar = inconsistency_check();
        if (inconsistency_dummyvar == 1)
        {
            return;
        }

        if (print_steps == true)
        {
            print_Matrix();
        }

        //'n' is the variable that represents which row we are scaling to create
        // a column of zeroes for the pivot
        for (int n{1}; n <= associated_y_dimension; n++)
        {

            if (n == m)
            { // Skip the row with the current pivot
                continue;
            }

            //'m' is both an x and y coord, we are scaling
            // based on the numbers above and below the pivot
            rr_scale_then_add(n, m, retrieve_value_from_entry(m, n)); // Scale this row so that the pivot will have a column of 0's

            inconsistency_dummyvar = inconsistency_check();
            if (inconsistency_dummyvar == 1)
            {
                return;
            }

            if (print_steps == true)
            {
                print_Matrix();
            }
        }
        //
        // END: Row reducer engine
        //
    }

    remove_garbage();

    print_Matrix();

    return;
}

void Matrix::remove_garbage()
{ // Sometimes, values in the order of 10^-32 appear. This is just to set them as 0.

    Entry *temp;
    temp = p_head;

    while (temp != nullptr)
    {

        if (temp->get_value() < 0.00001 && temp->get_value() > -0.00001)
        {

            temp->internal_set_value(0);
        }

        temp = temp->get_next();
    }

    temp = aug_p_head;

    while (temp != nullptr)
    {

        if ( (temp->get_value() < 0.00001) && (temp->get_value() > -0.00001) )
        {

            temp->internal_set_value(0);
        }

        temp = temp->get_next();
    }

    return;
}

int Matrix::inconsistency_check()
{

    int inconsistency_row{-1};
    int num_count{0};

    for (int i{1}; i <= associated_y_dimension; i++)
    {

        for (int j{1}; j <= associated_dimension; j++)
        {

            if (retrieve_value_from_entry(j, i) != 0)
            { // If the matrix has no numbers in it, but the augmented matrix does, there is an inconsistency

                num_count++;
                break; // This row is good, no need to check further
            }
        }

        if (num_count == 0)
        {

            inconsistency_row = i; // Preliminary. We still have to verify that this is not a row of zeroes situation

            for (int k{1}; k <= aug_xdim; k++)
            {

                if (aug_retrieve_value_from_entry(k, inconsistency_row) != 0)
                {

                    num_count++;
                    break;
                }
            }

            if (num_count != 0)
            {

                std::cout << std::endl;
                std::cout << "INCONSISTENCY DETECTED AT ROW " << inconsistency_row << std::endl;

                print_Matrix();

                return 1; // Error signal for the row_reduce function.
            }
        }

        inconsistency_row = -1;
        num_count = 0;
    }

    return 2; //"All is good" signal for the row_reduce function
}

void Matrix::rr_row_swap(int row_1_y_coord, int row_2_y_coord)
{ // Swap two rows

    double row_1_temp_var;
    double row_2_temp_var;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        row_1_temp_var = retrieve_value_from_entry(k, row_1_y_coord);
        row_2_temp_var = retrieve_value_from_entry(k, row_2_y_coord);

        edit_value(row_2_temp_var, k, row_1_y_coord);
        edit_value(row_1_temp_var, k, row_2_y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        row_1_temp_var = aug_retrieve_value_from_entry(k, row_1_y_coord);
        row_2_temp_var = aug_retrieve_value_from_entry(k, row_2_y_coord);

        aug_edit_value(row_2_temp_var, k, row_1_y_coord);
        aug_edit_value(row_1_temp_var, k, row_2_y_coord);
    }

    return;
}

void Matrix::rr_row_scale(int y_coord, double scalar)
{ // Multiply all entries of the row by a scalar

    if (scalar == 0)
    {
        return;
    }

    double new_value;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        new_value = (retrieve_value_from_entry(k, y_coord) * scalar);
        edit_value(new_value, k, y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        new_value = (aug_retrieve_value_from_entry(k, y_coord) * scalar);
        aug_edit_value(new_value, k, y_coord);
    }

    return;
}

void Matrix::rr_row_addition(int row_1_y_coord, int row_2_y_coord)
{ // Add the elements of row 2 on to row 1

    double temp_var1;
    double temp_var2;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        temp_var1 = retrieve_value_from_entry(k, row_1_y_coord);
        temp_var2 = retrieve_value_from_entry(k, row_2_y_coord);

        temp_var1 = temp_var1 - temp_var2;

        edit_value(temp_var1, k, row_1_y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        temp_var1 = aug_retrieve_value_from_entry(k, row_1_y_coord);
        temp_var2 = aug_retrieve_value_from_entry(k, row_2_y_coord);

        temp_var1 = temp_var1 - temp_var2;

        aug_edit_value(temp_var1, k, row_1_y_coord);
    }

    return;
}

void Matrix::rr_scale_then_add(int row_1_y_coord, int row_2_y_coord, double scalar)
{ // Combines row add and row scale, but the scale is reverted

    if (scalar == 0)
    {
        return;
    }

    rr_row_scale(row_2_y_coord, scalar);

    rr_row_addition(row_1_y_coord, row_2_y_coord);

    rr_row_scale(row_2_y_coord, 1 / scalar);

    return;
}

void Matrix::convert_to_fraction(double number)
{ // Taken from the web

    int cycles{10};
    double precision{5e-4};

    if (number == 0)
    {
        std::cout << 0;
        return;
    }

    int sign = number > 0 ? 1 : -1;
    number = number * sign; // abs(number);
    double new_number, whole_part;
    double decimal_part = number - (int)number;
    int counter = 0;

    std::valarray<double> vec_1{double((int)number), 1}, vec_2{1, 0}, temporary;

    while (decimal_part > precision & counter < cycles)
    {
        new_number = 1 / decimal_part;
        whole_part = (int)new_number;

        temporary = vec_1;
        vec_1 = whole_part * vec_1 + vec_2;
        vec_2 = temporary;

        decimal_part = new_number - whole_part;
        counter += 1;
    }

    if (vec_1[1] == 1)
    {
        std::cout << sign * number;
        return;
    }

    std::cout << sign * vec_1[0] << '/' << vec_1[1];
}

std::string Matrix::return_fraction_conversion(double number)
{ // Taken from the web

    std::string sign_string;
    std::string vec0_string;
    std::string slash_string;
    std::string vec1_string;

    std::string result;

    int cycles{10};
    double precision{5e-4};

    if (number == 0)
    {
        result = "0";
        return result;
    }

    int sign = number > 0 ? 1 : -1;
    number = number * sign; // abs(number);
    double new_number, whole_part;
    double decimal_part = number - (int)number;
    int counter = 0;

    std::valarray<double> vec_1{double((int)number), 1}, vec_2{1, 0}, temporary;

    while (decimal_part > precision & counter < cycles)
    {
        new_number = 1 / decimal_part;
        whole_part = (int)new_number;

        temporary = vec_1;
        vec_1 = whole_part * vec_1 + vec_2;
        vec_2 = temporary;

        decimal_part = new_number - whole_part;
        counter += 1;
    }

    if (vec_1[1] == 1)
    {

        sign_string = std::to_string(sign);
        vec0_string = std::to_string(vec_1[0]);

        result = sign_string + vec0_string;

        return result;
    }

    sign_string = std::to_string(sign);
    vec0_string = std::to_string(vec_1[0]);
    slash_string = "/";
    vec1_string = std::to_string(vec_1[1]);

    result = sign_string + vec0_string + slash_string + vec1_string;

    return result;
}

/////////////////////
// ENTRY DEFINITIONS
/////////////////////

Entry::Entry(int x, int y, double value)
{

    entry_value = value;
    entry_x_position = x;
    entry_y_position = y;

    p_next = nullptr;
}

Entry::~Entry()
{
    // empty
}

Entry *Entry::get_next() { return p_next; }

void Entry::set_next(Entry *p_new_next) { p_next = p_new_next; }

int Entry::get_x_pos() { return entry_x_position; }

int Entry::get_y_pos() { return entry_y_position; }

double Entry::get_value() { return entry_value; }

void Entry::internal_set_value(double value)
{

    entry_value = value;

    return;
}

//////////////////////////
// MatrixList DEFINITIONS
//////////////////////////

MatrixList::MatrixList()
{

    m_list_head = nullptr;
    number_of_matrices = 0;
}

MatrixList::~MatrixList() {} // does nothing

Matrix *MatrixList::get_list_head() { return m_list_head; }

void MatrixList::set_list_head(Matrix *p_new_list_head)
{
    m_list_head = p_new_list_head;
    return;
}

int MatrixList::get_number_of_matrices() { return number_of_matrices; }

Matrix *MatrixList::find_matrix(std::string id)
{

    Matrix *temp;
    temp = m_list_head;

    while (true)
    { // Use the LL functionality to move to the specified matrix id

        if (temp == nullptr)
        { // Matrix does not exist

            break;
        }

        if (temp->get_id() == id)
        {

            break;
        }
        else
        {

            temp = temp->get_next_matrix();
        }
    }

    return temp;
}

void MatrixList::insert_matrix_into_list(Matrix *new_matrix)
{

    Matrix *prev_temp = m_list_head;
    Matrix *temp = m_list_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next_matrix();
    }

    prev_temp->set_next_matrix(new_matrix);

    number_of_matrices++;

    return;
}

void MatrixList::new_matrix(std::string new_id, int new_xdim, int new_ydim)
{

    Matrix *p_new_matrix;

    p_new_matrix = new Matrix{new_id, new_xdim, new_ydim};

    if (number_of_matrices == 0)
    { // If this is the first matrix created, set it as the head

        m_list_head = p_new_matrix;
        number_of_matrices++;
    }
    else
    {

        insert_matrix_into_list(p_new_matrix);
    }

    p_new_matrix->create_matrix();

    return;
}

void MatrixList::print_list()
{

    std::string temp_str;

    Matrix *temp;
    temp = m_list_head;

    while (temp != nullptr)
    { // Until the end of the list is reached, print the id of each matrix

        temp_str = temp->get_id();
        std::cout << temp_str << std::endl;

        temp = temp->get_next_matrix();
    }

    return;
}

int main()
{

    std::cout << "Welcome to";
    Sleep(500);
    std::cout << ". ";
    Sleep(500);
    std::cout << ". ";
    Sleep(500);
    std::cout << ". " << std::endl;

    std::cout << " _____   ______          __  _____  ______ _____  _    _  _____ ______ _____  " << std::endl;
    std::cout << "|  __ \\ / __ \\ \\        / / |  __ \\|  ____|  __ \\| |  | |/ ____|  ____|  __ \\ " << std::endl;
    std::cout << "| |__) | |  | \\ \\  /\\  / /  | |__) | |__  | |  | | |  | | |    | |__  | |__) |" << std::endl;
    std::cout << "|  _  /| |  | |\\ \\/  \\/ /   |  _  /|  __| | |  | | |  | | |    |  __| |  _  / " << std::endl;
    std::cout << "| | \\ \\| |__| | \\  /\\  /    | | \\ \\| |____| |__| | |__| | |____| |____| | \\ \\ " << std::endl;
    std::cout << "|_|  \\_\\_____/   \\/  \\/     |_|  \\_\\______|_____/ \\____/ \\_____|______|_|  \\_\\ " << std::endl;

    std::cout << std::endl;

    std::cout << "A program by JACKSON TOTH" << std::endl;
    std::cout << "Last updated: March 20, 2023" << std::endl;

    MatrixList MyList{};
    std::string choice_variable;
    Matrix *p_temp_matrix;
    int PBR_find_id{0};

    while (true)
    {

        std::cout << std::endl;
        std::cout << "Menu: [S]elect matrix, [C]reate new matrix, [E]xit program" << std::endl;
        std::cout << "Please choose a command: ";
        std::cin >> choice_variable;

        if (choice_variable == "E" || choice_variable == "e")
        {

            break;
        }
        else if (choice_variable == "C" || choice_variable == "c")
        {

            std::cout << std::endl;

            int dimension{0};
            int y_dimension{0};

            std::string id;

            Matrix *temp;

            while (true)
            {
                std::cout << "Enter the ID of the Matrix: ";
                std::cin >> id;

                bool name_taken;
                temp = MyList.find_matrix(id);

                if (temp == nullptr)
                {
                    name_taken = false;
                }
                else
                {
                    name_taken = true;
                }

                if (id == "R" || id == "r" || id == "M" || id == "m")
                {
                    std::cout << "Forbidden matrix ID. Please choose another ID." << std::endl;
                    continue;
                }
                else if (name_taken == true)
                {
                    std::cout << "Matrix ID is already taken. Please choose another ID." << std::endl;
                    continue;
                }

                break;
            }

            while (1 > dimension || 1 > y_dimension)
            {

                std::cout << "Enter the x dimension of your matrix: ";
                std::cin >> dimension;

                if (dimension < 1)
                {
                    std::cout << "The x dimension cannot be less than 1." << std::endl;
                    continue;
                }

                std::cout << "Enter the y dimension of your matrix: ";
                std::cin >> y_dimension;

                if (y_dimension < 1)
                {
                    std::cout << "The y dimension cannot be less than 1." << std::endl;
                    continue;
                }
            }

            std::cout << "Creating matrix..." << std::endl;

            MyList.new_matrix(id, dimension, y_dimension);

            std::cout << std::endl;

            temp = MyList.find_matrix(id);

            temp->print_Matrix();

            std::cout << std::endl;

            temp->create_augmented_matrix();

            std::cout << std::endl;

            temp->print_Matrix();

            std::cout << "Matrix successfully created." << std::endl;

            continue;
        }
        else if (choice_variable == "S" || choice_variable == "s")
        {

            while (true)
            {

                std::cout << std::endl;

                std::cout << "---Begin Matrix List---" << std::endl;
                MyList.print_list();
                std::cout << "---End Matrix List---" << std::endl;

                std::cout << std::endl;

                std::cout << "Please choose a matrix (case sensitive) from the given ID's above. Enter [M] to return to menu: ";
                std::cin >> choice_variable;

                if (choice_variable == "M" || choice_variable == "m")
                {
                    break;
                }

                p_temp_matrix = MyList.find_matrix(choice_variable);

                if (p_temp_matrix == nullptr)
                { // See find_matrix function for details on this condition

                    std::cout << "Matrix not found. Please enter a valid matrix ID." << std::endl;
                    continue;
                }

                break;
            }

            if (choice_variable == "M" || choice_variable == "m")
            {
                continue;
            }

            std::cout << "Matrix found!" << std::endl;

            p_temp_matrix->print_Matrix();

            std::cout << "Matrix \"" << p_temp_matrix->get_id() << "\" selected" << std::endl;
            std::cout << "Please choose an action: [R]ow reduce, [E]dit entry, Return to [M]enu: ";

            std::cin >> choice_variable;

            if (choice_variable == "M" || choice_variable == "m")
            {

                continue;
            }
            else if (choice_variable == "R" || choice_variable == "r")
            {

                std::cout << "Would you like all intermediary matrices? (Prints every step of the row reduction), [Y] or [N]: ";
                std::cin >> choice_variable;

                if (choice_variable == "Y" || choice_variable == "y")
                {
                    p_temp_matrix->print_steps_true();
                }
                else if (choice_variable == "N" || choice_variable == "n")
                {
                    p_temp_matrix->print_steps_false();
                }
                else
                {
                    p_temp_matrix->print_steps_true();
                }

                p_temp_matrix->row_reduce();
            }
            else if (choice_variable == "E" || choice_variable == "e")
            {

                double xcoord;
                double ycoord;
                double new_value;

                std::cout << "Would you like to edit the matrix [M] or the augmented matrix [A]?: ";
                std::cin >> choice_variable;

                if (choice_variable == "M" || choice_variable == "m")
                {

                    std::cout << "Which x coordinate?: ";
                    std::cin >> xcoord;

                    std::cout << "Which y coordinate?: ";
                    std::cin >> ycoord;

                    std::cout << "What is the new value?: ";
                    std::cin >> new_value;

                    std::cout << "Updating entry (" << xcoord << ", " << ycoord << ") to " << new_value << std::endl;

                    p_temp_matrix->edit_value(new_value, xcoord, ycoord);

                    p_temp_matrix->print_Matrix();
                }
                else if (choice_variable == "A" || choice_variable == "a")
                {

                    std::cout << "Which x coordinate?: ";
                    std::cin >> xcoord;

                    std::cout << "Which y coordinate?: ";
                    std::cin >> ycoord;

                    std::cout << "What is the new value?: ";
                    std::cin >> new_value;

                    std::cout << "Updating entry (" << xcoord << ", " << ycoord << ") to " << new_value << " in the augmented matrix" << std::endl;

                    p_temp_matrix->aug_edit_value(new_value, xcoord, ycoord);

                    p_temp_matrix->print_Matrix();
                }
                else
                {
                    continue;
                }
            }

            std::string choice;

            while (choice != "E" || choice != "e" || choice != "M" || choice != "m")
            {

                std::cout << "Would you like to return to menu? Return to [M]enu, [E]xit Program: ";
                std::cin >> choice;

                if (choice != "e" && choice != "E" && choice != "m" && choice != "M")
                {
                    std::cout << "Invalid input." << std::endl;
                    std::cout << std::endl;
                }
                else
                {
                    break;
                }
            }

            if (choice == "E" || choice == "e")
            {
                break;
            }
            else if (choice == "M" || choice == "m")
            {
                continue;
            }
        }
    }

    return 0;
}
