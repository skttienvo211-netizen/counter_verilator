#include "Vcounter.h"           // Class được Verilator sinh ra từ counter.v
#include "verilated.h"          // Thư viện lõi
#include "verilated_vcd_c.h"    // Thư viện để xuất file sóng VCD

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    
    // Bật tính năng ghi dạng sóng (Trace)
    Verilated::traceEverOn(true);
    
    // Khởi tạo đối tượng mạch và đối tượng ghi sóng
    Vcounter* top = new Vcounter;
    VerilatedVcdC* tfp = new VerilatedVcdC;
    
    top->trace(tfp, 99);        // Liên kết mạch với file sóng
    tfp->open("dump.vcd");      // Đặt tên file xuất ra là dump.vcd

    int time = 0;               // Biến đếm thời gian ảo
    top->clk = 0;               // Tín hiệu ban đầu
    top->rst = 1;               // Bật Reset lúc đầu để xóa bộ đếm

    // Chạy mô phỏng đến thời điểm 210 (giống trong báo cáo của bạn)
    while (time < 210) {
        // Nhả tín hiệu Reset sau thời điểm 10
        if (time > 10) {
            top->rst = 0;
        }

        // Tạo xung nhịp: Cứ mỗi 5 đơn vị thời gian thì đảo trạng thái (0->1 hoặc 1->0)
        if (time % 5 == 0) {
            top->clk = !top->clk;
        }

        top->eval();            // Yêu cầu mạch tính toán lại giá trị
        tfp->dump(time);        // Ghi lại toàn bộ giá trị của mạch vào file VCD

        time++;                 // Tăng thời gian lên 1 bước
    }

    // Kết thúc và dọn dẹp bộ nhớ
    top->final();
    tfp->close();
    delete top;
    delete tfp;
    return 0;
}
