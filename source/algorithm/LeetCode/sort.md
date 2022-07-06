# 排序算法总结

## 1. 冒泡排序

冒泡排序算法的思想是不断比较相邻的元素，然后按照从小到大的顺序排列，如果大小顺序不一致，就交换两个元素的位置。由于会使得小的元素不断上升到数组的前部，因此叫冒泡排序。

同时，冒泡排序是稳定的，代表A=B的时候，排序前后A和B的相对顺序不会发生改变。

### 算法复杂度分析

冒泡排序中，每一轮都会有一个最小/最大的元素确定位置。
- 最差情况
  时间复杂度为：`O(1+2+...+n)=O(n^2)`
  空间复杂度为：`O(1)`
- 最好情况
  时间复杂度为：`O(n)`
  空间复杂度为：`O(1)`

### code


## 2. 选择排序

选择排序和冒泡排序非常相似，选择排序的每一轮会选定最小值，然后和当前位置的值进行交换，也是每一轮都会确定一个元素的位置。但是选择排序是不稳定的，也就是说，相同元素的相对位置在排序以后，是有可能发生改变的。

### 算法复杂度分析

选择排序中，每一轮都会有一个最小/最大的元素确定位置。
- 最差情况
  时间复杂度为：`O(1+2+...+n)=O(n^2)`
  空间复杂度为：`O(1)`
- 最好情况
  时间复杂度为：`O(1+2+...+n)=O(n^2)`
  空间复杂度为：`O(1)`

### code


## 3. 插入排序

插入排序的思想是，将数组分成两个部分，前面是已经排序好的，后面是未排序的。每次遍历未排序的元素，然后将其和已经排序的数组中的元素进行比较，然后放在合适的位置。

### 算法复杂度分析

插入排序中，每一个元素都需要和前面已经排序好的元素进行比较，并且会有很多的交换代替。
  
时间复杂度为：`O(1+2+...+n-1)=O(n^2)`

空间复杂度为：`O(1)`

### code


## 4. 归并排序

归并排序采用了分治合并的思想，先使得子序列有序，然后再将子序列合并成有序序列。归并排序是稳定排序，因为相等的元素是按照顺序写上去的。

算法的实现又两种方式，一种是递归法，一种是迭代法。

### 算法复杂度分析

归并排序中，需要额外的空间存储合并后的子序列，因此空间复杂度会高，并且每一个元素都比较了`log(n)`次。
  
时间复杂度为：`O(nlogn)`

空间复杂度为：`O(n)`


### code

#### 递归法

#### 迭代法


## 5. 快速排序

快排的思想是，从数组中挑选出一个元素，称为基准，然后重新排列元素，所有比元素值小的元素放在前面，大的放在后面。然后递归这两边的数组元素。

快排是不稳定的。

## 6. 希尔排序