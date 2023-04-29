# Khoai Phieu Luu Ki
<a name="ve-dau-trang"/>

* Họ tên: Vũ Thị Minh Thư
* Mã sinh viên: 22028116
* Lớp môn học: INT2215 23

##  Mục lục
* [1. Hướng dẫn cài đặt và chạy game](#cai-dat)
* [2. Mô tả chung về trò chơi](#mo-ta)
* [3. Các chức năng của trò chơi](#chuc-nang)
* [4. Các kĩ thuật lập trình được sử dụng](#ki-thuat)
* [5. Hỗ trợ](#ho-tro)
* [6. Kết luận](#ket-luan)
* [7. Tham Khảo](#tham-khao)

<a name="cai-dat"/>

## I, Hướng dẫn cài đặt và chạy game
  ### B1: Tải project về
  Cách 1: Trên Github, chọn Code => Download ZIP  
  Cách 2: Mở terminal tại thư mục muốn lưu project, sử dụng lệnh `git init` để khởi tạo Git, sau đó dùng lệnh `git clone https://github.com/VuThiMinhThu2004/Khoai-Phieu-Luu-Ki.git` để clone project về máy  
  ### B2: Cài đặt Make
  Tải và cài đặt Mingw32 
  Thêm đường dẫn đến thư mục bin vào `environment variable`  
  ### B3: Chạy game
  Mở terminal tại thư mục chứa project, sử dụng lệnh `mingw32-make` và `.\main` để chạy game
  
 <a name="mo-ta"/>

## II, Mô tả chung về trò chơi
  **Thể loại:** `Sinh tồn`, `phiêu lưu`
  
  [Video minh hoạ](https://youtu.be/LZU_LU0rsYk)
  Game sinh tồn, phiêu lưu trong thế giới 2D có thể di chuyển đánh quái.

  Người chơi hoá thân vào nhân vật trong game, di chuyển trên bản đồ và tấn công quái vật để kiếm số điểm cao nhất

  Người chơi có lượng máu tối đa là 100 và sẽ chết khi lượng máu trở về 0. Tốc độ di chuyển và sát thương của người chơi cao hơn quái vật

  Tiêu diệt một con quái vật sẽ được thêm 1 điểm
  

<a name = "chuc-nang"/>

### III, Các chức năng của trò chơi
  - Điều khiển nhân vật di chuyển bằng các phím `a`, `d`, `w` và tấn công bằng chuột
  - Hệ thống HP, đánh quái và quái tự tấn công người chơi
  - Quái vật tự đuổi theo người chơi khi vào phạm vi và tấn công
  - Chèn âm thanh vào mỗi lần click chuột(để bắn), thêm thời gian, điểm số vào game 
  - Ghi điểm cao và lấy điểm cao từ file text
  - Menu đơn giản, người chơi có thể chơi lại khi nhân vật chết
    
<a name = "ki-thuat"/>

### IV, Các kỹ thuật lập trình được sử dụng
  - Sử dụng thư viện đồ họa SDL
  - Lập trình hướng đối tượng (class) dùng để tách các thư viện 
  - Biến/kiểu dữ liệu/con trỏ/cấp phát động dùng trong các hàm trong thư viện 
  - Danh sách/nhiều đối tượng
  - Chia tách chương trình, có tách các thư viện một cách rõ ràng nhất có thể
  - Sử dụng các thuật toán cơ bản như thuật toán xử lý va chạm, animation,.... Ngoài ra em còn tự tìm hiểu và phát triển thêm thuật toán giúp quái tự động di chuyển, thuật toán đuổi theo và tấn công người chơi khi vào phạm vi(Sử dụng vector trong hệ toạ độ).
  - Hệ thống tile-map và có những chức năng riêng (collision, monster, map)

<a name = "ho-tro"/>

### V, Hỗ trợ
- Sử dụng Photoshop để edit ảnh
- Sử dụng PyxelEdit để làm map
- Sử dụng Aseprite để vẽ nhân vật, quái và các chi tiết của map

<a name = "ket-luan"/>

### VI, Kết luận
  - Sử dụng thư viện SDL 2.0
  - Tự thiết kế đồ họa cho game
  - Thiết kế map vô tận(từ các level đã tạo trước đó)

 <a name = "tham-khao"/>
    
 ### VII, Tham khảo
  - [Hướng dẫn](https://lazyfoo.net/tutorials/SDL/)
  - [Hướng dẫn 2](https://youtube.com/playlist?list=PLUFd3gYWwiYFdSoqZp35RU6mja4N5u4dz)
  - [Hướng dẫn 3](https://youtube.com/playlist?list=PLZd7ojlRK0bwrL8060OGurG_kaeJc_QOH)
  - [Hướng dẫn 4](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx)
  - [Đồ hoạ (Tự thiết kế và có lấy ý tưởng đồ họa từ)](https://www.youtube.com/watch?v=k0vZShmhG74)
  - [Làm map](https://www.youtube.com/watch?v=5f-g87aGbBc)
    
[về đầu trang](#ve-dau-trang)
