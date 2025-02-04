def mergesort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2  # Finding the mid of the array
        left = arr[:mid]  # Dividing the elements into 2 halves
        right = arr[mid:]

        mergesort(left)  # Sorting the first half
        mergesort(right)  # Sorting the second half

        i = j = k = 0

        # Merging the sorted halves
        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                arr[k] = left[i]
                i += 1
            else:
                arr[k] = right[j]
                j += 1
            k += 1

        # Checking if any element was left
        while i < len(left):
            arr[k] = left[i]
            i += 1
            k += 1

        while j < len(right):
            arr[k] = right[j]
            j += 1
            k += 1


def main():
    # Input values as integers
    input_list = input("Enter numbers, separated by ',': ").split(',')
    input_list = [int(x.strip()) for x in input_list]

    print(f"Initial list: {input_list}")

    mergesort(input_list)

    print(f"Sorted list: {input_list}")


if __name__ == "__main__":
    main()
