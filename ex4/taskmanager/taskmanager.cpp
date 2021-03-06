#include "taskmanager.h"
#include "ui_taskmanager.h"


TaskManager::TaskManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskManager)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateUsage()));
    timer->start(10);
    Init();
}

void TaskManager::Init(){
    ui->tableWidget->setRowCount(20);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"applications"<<"pid"<<"stat"<<"priority"<<"nice");
    ui->tableWidget->horizontalHeader()->resizeSection(0,200); //设置表头第一列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(1,110); //设置表头第一列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(2,110); //设置表头第一列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(3,110); //设置表头第一列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(4,110); //设置表头第一列的宽度为150
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
}


void TaskManager::UpdateUsage(){
    //UpdateMenUsage();

    //UpdateCpuUsage();

    //UpdateDiskUsage();
}

void TaskManager::UpdateMenUsage(){
    QFile file;
    QString str;
    double total, used;

    //内存信息
    file.setFileName("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)) return;
    //
    str = file.readLine();
    total = str.mid(10, str.size() - 13).trimmed().toInt()/(1024*1024.0);
    ui->label_memtotal->setText("内存大小:" + QString::number(total, 'f', 1) + "GB");
    //
    str = file.readLine();
    used = total - str.mid(10, str.size() - 13).trimmed().toInt()/(1024*1024.0);
    ui->label_memused->setText("内存used:" + QString::number(used, 'f', 1) + "GB");
    //
    ui->progressBar_mem->setValue(used*100/total);
    file.close();
}

void TaskManager::UpdateCpuUsage(){
    string tmp;
    double usage;
    int total1, total2, user, nice, system, idle1, idle2,
            iowait, irq, softirq, stealstolen, guest, guest_nice;
    int cpu0_total1, cpu0_total2, cpu0_idle1, cpu0_idle2,
            cpu1_total1, cpu1_total2, cpu1_idle1, cpu1_idle2,
            cpu2_total1, cpu2_total2, cpu2_idle1, cpu2_idle2,
            cpu3_total1, cpu3_total2, cpu3_idle1, cpu3_idle2;
    ifstream fin("/proc/stat", ios::in);
    fin >> tmp >> user >> nice >> system >> idle1
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    total1 = user + nice + system + idle1 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu0
    fin >> tmp >> user >> nice >> system >> cpu0_idle1
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu0_total1 = user + nice + system + cpu0_idle1 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu1
    fin >> tmp >> user >> nice >> system >> cpu1_idle1
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu1_total1 = user + nice + system + cpu1_idle1 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu2
    fin >> tmp >> user >> nice >> system >> cpu2_idle1
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu2_total1 = user + nice + system + cpu2_idle1 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu3
    fin >> tmp >> user >> nice >> system >> cpu3_idle1
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu3_total1 = user + nice + system + cpu3_idle1 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;

    fin.close();

    usleep(1000000);

    fin.open("/proc/stat", ios::in);
    fin >> tmp >> user >> nice >> system >> idle2 >> iowait >>
            irq >>softirq >> stealstolen >> guest >> guest_nice;
    total2 = user + nice + system + idle2 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu0
    fin >> tmp >> user >> nice >> system >> cpu0_idle2
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu0_total2 = user + nice + system + cpu0_idle2 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu1
    fin >> tmp >> user >> nice >> system >> cpu1_idle2
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu1_total2 = user + nice + system + cpu1_idle2 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu2
    fin >> tmp >> user >> nice >> system >> cpu2_idle2
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu2_total2 = user + nice + system + cpu2_idle2 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;
    //cpu3
    fin >> tmp >> user >> nice >> system >> cpu3_idle2
            >> iowait >> irq >> softirq >> stealstolen >> guest >> guest_nice;
    cpu3_total2 = user + nice + system + cpu3_idle2 + iowait +
            irq + softirq + stealstolen + guest + guest_nice;

    usage = (total2 + idle1 - total1 - idle2) * 1.0 / (total2 - total1);
    ui->progressBar_cpu->setValue(usage * 100);
    usage = (cpu0_total2 + cpu0_idle1 - cpu0_total1 - cpu0_idle2) * 1.0 / (cpu0_total2 - cpu0_total1);
    ui->progressBar_cpu0->setValue(usage * 100);
    usage = (cpu1_total2 + cpu1_idle1 - cpu1_total1 - cpu1_idle2) * 1.0 / (cpu1_total2 - cpu1_total1);
    ui->progressBar_cpu1->setValue(usage * 100);
    usage = (cpu2_total2 + cpu2_idle1 - cpu2_total1 - cpu2_idle2) * 1.0 / (cpu2_total2 - cpu2_total1);
    ui->progressBar_cpu2->setValue(usage * 100);
    usage = (cpu3_total2 + cpu3_idle1 - cpu3_total1 - cpu3_idle2) * 1.0 / (cpu3_total2 - cpu3_total1);
    ui->progressBar_cpu3->setValue(usage * 100);

    fin.close();
}

void TaskManager::UpdateDiskUsage(){
    string str;
    int st;
    int use1[10],use2[10], count1 = 0, count2 = 0, t = 0;
    ifstream fin("/proc/diskstats", ios::in);
     while(!fin.eof()){
        fin >> str >> str >> str;
        if(str.size() == 3 && str[0] == 's' && str[1] == 'd'){
            for(int j = 0; j < 9; j++)
                fin >> str;
            fin >> use1[count1++];
            if(count1 == 3) break;
            getline(fin, str);
        }
        else getline(fin, str);
    }
    fin.close();

    usleep(1000000);

    fin.open("/proc/diskstats", ios::in);
    while(!fin.eof()){
        fin >> str >> str >> str;
        if(str.size() == 3 && str[0] == 's' && str[1] == 'd'){
            for(int j = 0; j < 9; j++)
                fin >> str;
            fin >> use2[count2++];
            if(count2 == 3) break;
            getline(fin, str);
        }
        else getline(fin, str);
    }
    fin.close();

    for(int i = 0; i < 3; i++){
        switch(i){
            case 0:
                ui->progressBar_disk1->setValue((use2[0] - use1[0]) / 10);
                cout << "use2:" << use2[0] << "   use1:" << use1[0] << endl;
                break;
            case 1:
                ui->progressBar_disk2->setValue((use2[1] - use1[1]) / 10);
                cout << "use2:" << use2[1] << "   use1:" << use1[1] << endl;
                break;
            case 2:
                ui->progressBar_disk3->setValue((use2[2] - use1[2]) / 10);
                cout << "use2:" << use2[2] << "   use1:" << use1[2] << endl;
                break;
        }
    }
}


void TaskManager::UpdateProc(int pid){
    string name, stat;
    int tmp, nice, priority;
    stringstream ss;
    ss << pid;

    ifstream fin("/proc/" + ss.str() + "/stat", ios::in);
    fin >> tmp >> name;
    fin.close();

    fin.open("/proc/" + ss.str() + "/stat", ios::in);
}

TaskManager::~TaskManager()
{
    delete ui;
    delete timer;
}
