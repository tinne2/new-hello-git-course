def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    
    return merge(left, right)

def merge(left, right):
    merged = []
    while left and right:
        if left[0] < right[0]:
            merged.append(left.pop(0))
        else:
            merged.append(right.pop(0))
    merged.extend(left)
    merged.extend(right)
    return merged

if __name__ == "__main__":
    # Take input from user
    input_string = input("Enter numbers, separated by ',': ")
    
    # Convert input to a list of integers
    value_list = list(map(int, input_string.split(',')))
    
    # Call merge_sort and print the result
    sorted_list = merge_sort(value_list)
    
    # Print the sorted list (this is the expected output)
    print(sorted_list)
