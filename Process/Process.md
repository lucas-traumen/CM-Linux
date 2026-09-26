# Thao tác với Tiến trình – Process

Tài liệu này tổng hợp kiến thức nền và các bài tập của mục **Process**, dựa trên slide `03. Process.pdf` và mã ví dụ trong thư mục `day_3/`.

## 1. Tiến trình là gì

Tiến trình (process) là một chương trình đang được thực thi, bao gồm mã lệnh, dữ liệu, stack, heap và trạng thái thực thi do kernel quản lý. Mỗi tiến trình được định danh bằng:

* **PID** (`getpid()`) — số định danh duy nhất của tiến trình.
* **PPID** (`getppid()`) — PID của tiến trình cha đã tạo ra nó.

Tiến trình đầu tiên khi hệ thống khởi động là `init`/`systemd` (PID 1), mọi tiến trình khác đều là hậu duệ của nó.

## 2. Vòng đời và trạng thái tiến trình

Một tiến trình đi qua các trạng thái chính:

| Trạng thái | Ký hiệu trong `ps` | Ý nghĩa |
|---|---|---|
| Running | `R` | Đang được CPU thực thi |
| Sleeping | `S`, `D` | Chờ sự kiện (I/O, timer, tín hiệu) |
| Stopped | `T` | Bị dừng bởi tín hiệu (VD: `SIGSTOP`) |
| Zombie | `Z`, `<defunct>` | Đã kết thúc nhưng chưa được cha thu thập |
| Orphan | — | Cha đã chết, được `init` nhận nuôi |

Vòng đời cơ bản nhất: **tạo ra → thực thi → kết thúc → được tiến trình cha chờ đợi (thu dọn)**.

## 3. Tạo tiến trình với `fork()`

`fork()` nhân bản tiến trình gọi nó thành hai tiến trình gần như giống hệt nhau, và trả về **một lần trong mỗi tiến trình**:

* `= -1` — tạo thất bại, không có tiến trình con.
* `= 0` — đang chạy trong **tiến trình con**.
* `> 0` — đang chạy trong **tiến trình cha**; giá trị trả về là PID của con.

```c
pid_t pid = fork();

if (pid < 0) {
    perror("Fork failed");
    return 1;
} else if (pid == 0) {
    // Tiến trình con
} else {
    // Tiến trình cha, pid = PID của con
}
```

Sau `fork()`, con nhận bản sao của không gian địa chỉ cha (ngày nay kernel dùng *copy-on-write* nên việc sao chép chỉ xảy ra khi có ghi). File descriptor được chia sẻ, thứ tự chạy giữa cha và con không xác định.

Xem ví dụ đầy đủ: `day_3/fork_wait.c`, `day_3/fork_execl.c`.

## 4. Chờ tiến trình con: `wait()`, `waitpid()`

Khi con kết thúc, kernel giữ lại thông tin kết thúc (exit status) cho tới khi cha gọi `wait()`. Nếu không, con trở thành zombie.

```c
#include <sys/wait.h>

pid_t wait(int *status);            // chờ con bất kỳ kết thúc
pid_t waitpid(pid_t pid, int *status, int options);
```

* `waitpid(pid, &status, 0)` — chờ đúng tiến trình `pid`, **khởi** (blocking).
* `waitpid(pid, &status, WNOHANG)` — không khởi, trả `0` nếu con vẫn đang chạy; phù hợp polling (xem `day_3/waitpid_nohang.c`).

Đọc exit status qua các macro:

| Macro | Ý nghĩa |
|---|---|
| `WIFEXITED(status)` | Con kết thúc bình thường qua `exit()`/`return` |
| `WEXITSTATUS(status)` | Mã thoát (8 bit dưới) của con |
| `WIFSIGNALED(status)` | Con bị kết thúc bởi tín hiệu |
| `WTERMSIG(status)` | Tín hiệu đã kết thúc con |

`status` là giá trị do **kernel ghi vào** khi con kết thúc — trước `wait()` nó vẫn giữ giá trị rác cũ (xem `day_3/status.c`).

Ví dụ chờ từng con với mã thoát cụ thể: `day_3/waitpid.c`.

## 5. Thay thế mã thực thi: họ hàm `exec()`

`exec()` thay thế **toàn bộ** không gian địa chỉ (mã, dữ liệu, heap, stack) của tiến trình hiện tại bằng một chương trình mới, nhưng **PID không đổi**. Nếu gọi thành công, mọi dòng lệnh sau nó trong cùng hàm không còn ý nghĩa (chỉ chạy được khi `exec` thất bại).

Các biến thể:

| Hàm | Tìm file theo | Cách truyền đối số |
|---|---|---|
| `execl(path, arg0, ..., NULL)` | đường dẫn tuyệt đối/tương đối | danh sách (list) |
| `execlp(file, arg0, ..., NULL)` | qua `PATH` | danh sách |
| `execv(path, argv[])` | đường dẫn | mảng (vector) |
| `execvp(file, argv[])` | qua `PATH` | mảng |

```c
execl("/bin/ls", "ls", "-l", NULL);
perror("execl failed");   // chỉ chạy khi exec thất bại
exit(1);
```

Xem `day_3/fork_execl.c` — mẫu kinh điển: con `exec()` để chạy lệnh mới, cha `wait()` để thu dọn.

### Biến môi trường

Tiến trình con **kế thừa** môi trường của cha, nên cha có thể truyền thông tin qua biến môi trường:

```c
setenv("MY_COMMAND", "ls", 1);   // tiến trình cha thiết lập
...
char *cmd = getenv("MY_COMMAND"); // tiến trình con đọc
execlp(cmd, cmd, NULL);
```

## 6. Tham số dòng lệnh: `argc` / `argv`

Mỗi tiến trình nhận đối số dòng lệnh qua hàm `main`:

```c
int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
    return 0;
}
```

`argc` là số đối số (tối thiểu 1 — tên chương trình), `argv[0]` là tên chương trình. Xem `day_3/argument.c`.

## 7. Trạng thái đặc biệt: Zombie và Orphan

### Zombie (tiến trình "ma")

Xảy ra khi **con đã kết thúc nhưng cha chưa gọi `wait()`**: kernel giữ lại một entry tiến trình tối giản chỉ để lưu exit status, hiển thị `<defunct>` trong `ps`.

```c
if (pid == 0) exit(0);          // con thoát ngay
else { sleep(30); }             // cha ngủ, không wait() → con thành zombie
```

Quan sát khi chương trình đang chạy:

```bash
ps aux | grep <pid_con>    # trạng thái Z, <defunct>
```

Zombie không chiếm CPU/ram thực sự nhưng làm đầy bảng tiến trình nếu tích tụ nhiều. Khi cha kết thúc, zombie được `init` dọn dẹp. Xem `day_3/zombie.c`.

### Orphan (tiến trình mồ côi)

Xảy ra khi **cha chết trước con**: con bị `init`/`systemd` (PID 1) nhận nuôi, do đó PPID của con đổi thành `1`.

```c
if (pid == 0) {
    printf("Child: PID=%d, PPID=%d\n", getpid(), getppid());
    sleep(5);
    printf("Child: PID=%d, PPID=%d (được init nhận nuôi)\n",
           getpid(), getppid());   // PPID giờ là 1
}
else {
    sleep(2);
    // cha thoát trước → con thành orphan
}
```

Orphan tiếp tục chạy bình thường, chỉ khác cha. Xem `day_3/orphan.c`.

## 8. Bài tập

### Bài 1: Khởi tạo và thu dọn tiến trình

* Cha tạo con bằng `fork()`; in ra PID của chính nó và PID của con, sau đó `wait()` chờ con; dùng `WIFEXITED()`/`WEXITSTATUS()` để in mã thoát của con.
* Con in PID của chính nó rồi `exit(10)`.

Tham khảo: `day_3/status.c`, `day_3/fork_wait.c`.

Xem mã nguồn đầy đủ: [assigment1.c](assigment1.c)


### Bài 2: Thay thế mã thực thi và tương tác với môi trường

* Cha thiết lập biến môi trường `MY_COMMAND` (VD: `ls`).
* Con đọc biến môi trường này bằng `getenv()` và dùng `execlp()` để thực thi lệnh tương ứng.
* Câu hỏi: sau khi `exec()` thành công, không gian địa chỉ và mã lệnh cũ của tiến trình con còn lại gì?

Tham khảo: `day_3/fork_execl.c`.
Xem mã nguồn đầy đủ: [assigment2.c](assigment2.c)

``` Khi thực hiện đặt biến môi trường có 2 cái trường hợp khi fork xong nếu thay đổi môi trường nó không ảnh hưởng đến tiến trình khác ``` 

### Bài 3: Khảo sát trạng thái Zombie và Orphan

* Zombie: con thoát ngay, cha `sleep()` lâu và không `wait()`; dùng `ps` quan sát trạng thái `<defunct>`.
* Orphan: cha thoát ngay sau khi tạo con; con `sleep()` và in PPID liên tục, quan sát PPID đổi thành `1`.
* Câu hỏi: vì sao hai trạng thái này xuất hiện và ý nghĩa của chúng trong Linux?

Tham khảo: `day_3/zombie.c`, `day_3/orphan.c`.

Xem mã nguồn đầy đủ: [assigment3.c](assigment3.c)

``` Ở đây ta nhận thấy là khi nói đến zombie là parent không wait và giải phóng child nên trong lúc quá trình chạy child die lúc này xuất hiện zombie và khi parent die lúc này parent hệ thống sẽ wait zombie  và dọn dẹp ```

```text
lucas@lucas-Dell-G15-5525:~/Desktop/Github/CM-Linux/Process$ ps aux | grep assigment3
lucas    2222481  0.0  0.0   2692  1656 pts/3    S+   14:53   0:00 ./assigment3
lucas    2222482  0.0  0.0      0     0 pts/3    Z+   14:53   0:00 [assigment3] <defunct>
lucas    2222539  0.0  0.0   9156  2300 pts/4    S+   14:53   0:00 grep --color=auto assigment3
lucas@lucas-Dell-G15-5525:~/Desktop/Github/CM-Linux/Process$ ^C
lucas@lucas-Dell-G15-5525:~/Desktop/Github/CM-Linux/Process$ ps aux | grep assigment3
lucas    2224096  0.0  0.0   9156  2304 pts/4    S+   14:54   0:00 grep --color=auto assigment3
```

```Với orphan parent die trước child lúc này nó sẽ nhờ parent hệ thống giám sát ```

```text
Parent: Waiting for child 2237155
Child: PID=2237155, PPID=2237154
Parent: Child finished!
lucas@lucas-Dell-G15-5525:~/Desktop/Github/CM-Linux/Process$ Child after 5s: PID=2237155, PPID=4436
```


## 9. Build và chạy

Các ví dụ trong `day_3/` chỉ dùng thư viện chuẩn, biên dịch trực tiếp bằng `gcc`:

```bash
gcc day_3/fork_wait.c -o fork_wait && ./fork_wait
gcc day_3/waitpid.c -o waitpid && ./waitpid
gcc day_3/zombie.c -o zombie && ./zombie     # mở terminal khác: ps aux | grep zombie
gcc day_3/orphan.c -o orphan && ./orphan
```
