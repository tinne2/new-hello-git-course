def debug_print(**kwargs):
    for key, value in kwargs.items():
        print("{}: {}".format(key, value))

def mergesort(array):
    debug_print(array=array)  # Debugging the array at the start
    if len(array) <= 1:
        return array

    m = len(array) // 2
    debug_print(m=m)  # Debugging the middle index

    left = mergesort(array[:m])  # Recursively sort the left half
    right = mergesort(array[m:])  # Recursively sort the right half

    return merge(left, right)  # Merge the sorted halves

def merge(left, right):
    debug_print(debug_msg="Merging...", left=left, right=right)  # Debugging the merge step

    merged = []

    while len(left) > 0 and len(right) > 0:
        if left[0] <= right[0]:
            merged.append(left.pop(0))
        else:
            merged.append(right.pop(0))

    if len(left) > 0:
        merged += left  # If there are any remaining elements in left, add them
    if len(right) > 0:
        merged += right  # If there are any remaining elements in right, add them

    debug_print(merged=merged)  # Debugging the merged result
    return merged

if __name__ == "__main__":
    input_str = input("Enter numbers, separated by ',': ")

    # Convert input string to a list of integers
    value_list = [int(x.strip()) for x in input_str.split(',')]

    debug_print(value_list=value_list)  # Debugging the input values

    sorted_list = mergesort(value_list)
    print("Sorted list:", sorted_list)  # Printing the final sorted list
