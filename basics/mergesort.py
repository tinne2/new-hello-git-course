def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    
    return merge(left, right)

def merge(left, right):
    merged = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            merged.append(left[i])
            i += 1
        else:
            merged.append(right[j])
            j += 1
    merged.extend(left[i:])
    merged.extend(right[j:])
    return merged

if __name__ == "__main__":
    # Take input from user
    input_string = input("Enter numbers, separated by ',': ")
    
    try:
        # Convert input to a list of integers
        value_list = list(map(int, input_string.split(',')))
        
        # Call merge_sort and print the result
        sorted_list = merge_sort(value_list)
        
        # Print the sorted list (this is the expected output)
        print("Sorted list:", sorted_list)
    except ValueError:
        print("Please enter a valid list of numbers separated by commas.")
