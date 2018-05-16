figure(1)
hold on
subplot(3,1,1)
plot(frame,errorX,'r','LineWidth',1)
x1=xlabel('图像帧数');
y1=ylabel('X轴误差/m');
set(x1,'Fontname','宋体','FontSize',22)
set(y1,'Fontname','宋体','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['X均值:',num2str(mean(abs(errorX)))])
disp(['X方差:',num2str(var(errorX,1))])
disp(['X最大值:',num2str(max(abs(errorX)))])

subplot(3,1,2)
plot(frame,errorY,'r','LineWidth',1)
x2=xlabel('图像帧数');
y2=ylabel('Y轴误差/m');
set(x2,'Fontname','宋体','FontSize',22)
set(y2,'Fontname','宋体','FontSize',22)
set(gca,'FontSize',22);
disp(['Y均值:',num2str(mean(abs(errorY)))])
disp(['Y方差:',num2str(var(errorY,1))])
disp(['Y最大值:',num2str(max(abs(errorY)))])

subplot(3,1,3)
plot(frame,errorZ,'r','LineWidth',1)
x3=xlabel('图像帧数');
y3=ylabel('Z轴误差/m');
set(x3,'Fontname','宋体','FontSize',22)
set(y3,'Fontname','宋体','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['Z均值:',num2str(mean(abs(errorZ)))])
disp(['Z方差:',num2str(var(errorZ,1))])
disp(['Z最大值:',num2str(max(abs(errorZ)))])

figure(2)
hold on
timeCost(1:6)=timeCost(20:25);
timeCostNoROI(1:6)=timeCostNoROI(20:25);
plot(frame,timeCost,'r','LineWidth',1)
plot(frame,timeCostNoROI,'b','LineWidth',1)
x4=xlabel('图像帧数');
y4=ylabel('时间消耗/s');
l4=legend('启用追踪','关闭追踪');
set(y4,'Fontname','宋体','FontSize',22)
set(x4,'Fontname','宋体','FontSize',22)
set(l4,'Fontname','宋体','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['T1均值:',num2str(mean(abs(timeCost)))])
disp(['T1方差:',num2str(var(timeCost,1))])
disp(['T1最大值:',num2str(max(abs(timeCost)))])
disp(['T2均值:',num2str(mean(abs(timeCostNoROI)))])
disp(['T2方差:',num2str(var(timeCostNoROI,1))])
disp(['T2最大值:',num2str(max(abs(timeCostNoROI)))])