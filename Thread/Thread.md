# Thao tác với Luồng – Thread

Tài liệu này tổng hợp kiến thức nền và các bài tập của mục **Thread**, dựa trên slide `04. Thread.pdf` và mã ví dụ trong thư mục `day_4/`.

## 1. Luồng là gì

Luồng (thread) là đơn vị thực thi nhỏ nhất trong một tiến trình, chia sẻ không gian địa chỉ (mã, dữ liệu, heap) với các luồng khác trong cùng tiến trình nhưng có stack riêng. So với tiến trình:

| Đặc điểm | Tiến trình | Luồng |
|---|---|---|
| Không gian địa chỉ | Riêng biệt | Chia sẻ trong cùng tiến trình |
| Tạo mới | Tốn kém (`fork()`) | Nhẹ hơn nhiều (`pthread_create()`) |
| Giao tiếp | IPC (pipe, shm, signal) | Đọc/ghi biến chung trực tiếp |
| Rủi ro | Cách ly tốt | Race condition nếu không đồng bộ |

Mỗi luồng được định danh bằng `pthread_t`, truy cập qua `pthread_self()`.

## 2. Tạo và quản lý luồng với POSIX threads

### Tạo luồng: `pthread_create()`

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
```

* `thread` — con trỏ nhận ID luồng mới.
* `attr` — thuộc tính luồng (thường truyền `NULL` để dùng mặc định).
* `start_routine` — hàm mà luồng sẽ chạy, nhận `void*` và trả về `void*`.
* `arg` — đối số truyền vào `start_routine`.

Xem ví dụ cơ bản: `day_4/basic_thread.c`.

### Truyền đối số cho luồng

Vì `start_routine` chỉ nhận một `void*`, cần đóng gói nhiều tham số vào struct hoặc truyền con trỏ đến biến:

```c
void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d running\n", id);
    return NULL;
}
```

**Lưu ý**: nếu truyền con trỏ đến biến cục bộ của luồng cha, phải đảm bảo biến đó còn sống khi luồng con đọc. Xem `day_4/thread_with_args.c`.

### Kết thúc luồng

Luồng kết thúc khi:

* Hàm `start_routine` trả về (giá trị trả về có thể thu thập qua `pthread_join`).
* Gọi `pthread_exit(retval)` — tương tự `exit()` nhưng chỉ dừng luồng hiện tại.
* Bị hủy bởi `pthread_cancel()` (ít dùng, khó kiểm soát).

Xem `day_4/thread_exit.c`.

## 3. Join và Detach

### Join: chờ luồng kết thúc

```c
int pthread_join(pthread_t thread, void **retval);
```

Luồng gọi `join` sẽ **chặn** cho đến khi luồng đích kết thúc và thu thập giá trị trả về. Mặc định luồng ở trạng thái *joinable*. Xem `day_4/detach_vs_join.c`.

### Detach: luồng tự dọn dẹp

```c
int pthread_detach(pthread_t thread);
```

Luồng *detached* tự giải phóng tài nguyên khi kết thúc, không thể `join`. Phù hợp luồng nền không cần thu thập kết quả.

**So sánh**:

| Trạng thái | Thu thập kết quả | Tự dọn dẹp | Dùng khi |
|---|---|---|---|
| Joinable (mặc định) | Có, qua `pthread_join()` | Không | Cần biết kết quả/thời điểm kết thúc |
| Detached | Không | Có | Lửa-và-quên (fire-and-forget) |

## 4. Định danh và so sánh luồng

`pthread_self()` trả về `pthread_t` của luồng đang chạy. Để so sánh hai `pthread_t`:

```c
int pthread_equal(pthread_t t1, pthread_t t2);
```

Không dùng `==` vì `pthread_t` có thể là struct. Xem `day_4/thread_self_equal.c`.

## 5. Đồng bộ hóa: Mutex

Khi nhiều luồng đọc/ghi biến chung, cần mutex để tránh race condition:

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&mutex);
shared_counter++;
pthread_mutex_unlock(&mutex);
```

Nếu không khóa, thứ tự xen kẽ giữa các luồng gây kết quả sai — xem minh họa trong `day_4/race_condition.c`.

## 6. Đồng bộ hóa: Condition Variable

Condition variable cho phép luồng **chờ** một điều kiện trở thành đúng, thay vì busy-wait:

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_lock(&mutex);
while (ready == 0)
    pthread_cond_wait(&cond, &mutex);
pthread_mutex_unlock(&mutex);
```

* `pthread_cond_signal(&cond)` — đánh thức **một** luồng đang chờ.
* `pthread_cond_broadcast(&cond)` — đánh thức **tất cả** luồng đang chờ.

**Quan trọng**: luôn kiểm tra điều kiện trong vòng `while`, không phải `if`, vì spurious wakeup có thể xảy ra. Xem `day_4/basic_condvar.c`.

## 7. Bài tập

### Bài 1: Tạo luồng cơ bản và truyền đối số

* Tạo 3 luồng, mỗi luồng nhận một số nguyên làm ID và in ra ID cùng PID của tiến trình.
* Luồng chính `join` tất cả luồng con trước khi thoát.
* Câu hỏi: các luồng con có cùng PID không? Vì sao?

Tham khảo: `day_4/basic_thread.c`, `day_4/thread_with_args.c`.

### Bài 2: Race condition và bảo vệ bằng mutex

* Tạo nhiều luồng cùng tăng một biến đếm chung 1 triệu lần mỗi luồng.
* Chạy không mutex → kết quả sai; chạy có mutex → kết quả đúng.
* Câu hỏi: tại sao kết quả không-mutex thay đổi mỗi lần chạy?

Tham khảo: `day_4/race_condition.c`.

### Bài 3: Signal vs Broadcast với condition variable

* Tạo 3 luồng waiter chờ trên cùng một condition variable.
* Dùng `pthread_cond_signal()` → quan sát chỉ một luồng tỉnh giấc mỗi lần.
* Dùng `pthread_cond_broadcast()` → quan sát tất cả luồng tỉnh giấc cùng lúc.
* Câu hỏi: khi nào nên dùng signal, khi nào nên dùng broadcast?

Tham khảo: `day_4/basic_condvar.c`.

### Bài 4: Joinable vs Detached

* Tạo một luồng joinable, `join` nó từ luồng chính và in giá trị trả về.
* Tạo một luồng detached, xác nhận không thể `join` (trả lỗi `EINVAL`).
* Câu hỏi: nếu `join` một luồng đã detach thì xảy ra gì?

Tham khảo: `day_4/detach_vs_join.c`.

## 8. Build và chạy

Các ví dụ trong `day_4/` dùng thư viện pthreads, biên dịch bằng `gcc` với cờ `-lpthread`:

```bash
gcc day_4/basic_thread.c -o basic_thread -lpthread && ./basic_thread
gcc day_4/thread_with_args.c -o thread_with_args -lpthread && ./thread_with_args
gcc day_4/race_condition.c -o race_condition -lpthread && ./race_condition
gcc day_4/basic_condvar.c -o basic_condvar -lpthread && ./basic_condvar
gcc day_4/detach_vs_join.c -o detach_vs_join -lpthread && ./detach_vs_join
```

</content>