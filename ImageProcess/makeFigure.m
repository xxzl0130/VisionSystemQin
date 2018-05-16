figure(1)
hold on
subplot(3,1,1)
plot(frame,errorX,'r','LineWidth',1)
x1=xlabel('ͼ��֡��');
y1=ylabel('X�����/m');
set(x1,'Fontname','����','FontSize',22)
set(y1,'Fontname','����','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['X��ֵ:',num2str(mean(abs(errorX)))])
disp(['X����:',num2str(var(errorX,1))])
disp(['X���ֵ:',num2str(max(abs(errorX)))])

subplot(3,1,2)
plot(frame,errorY,'r','LineWidth',1)
x2=xlabel('ͼ��֡��');
y2=ylabel('Y�����/m');
set(x2,'Fontname','����','FontSize',22)
set(y2,'Fontname','����','FontSize',22)
set(gca,'FontSize',22);
disp(['Y��ֵ:',num2str(mean(abs(errorY)))])
disp(['Y����:',num2str(var(errorY,1))])
disp(['Y���ֵ:',num2str(max(abs(errorY)))])

subplot(3,1,3)
plot(frame,errorZ,'r','LineWidth',1)
x3=xlabel('ͼ��֡��');
y3=ylabel('Z�����/m');
set(x3,'Fontname','����','FontSize',22)
set(y3,'Fontname','����','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['Z��ֵ:',num2str(mean(abs(errorZ)))])
disp(['Z����:',num2str(var(errorZ,1))])
disp(['Z���ֵ:',num2str(max(abs(errorZ)))])

figure(2)
hold on
timeCost(1:6)=timeCost(20:25);
timeCostNoROI(1:6)=timeCostNoROI(20:25);
plot(frame,timeCost,'r','LineWidth',1)
plot(frame,timeCostNoROI,'b','LineWidth',1)
x4=xlabel('ͼ��֡��');
y4=ylabel('ʱ������/s');
l4=legend('����׷��','�ر�׷��');
set(y4,'Fontname','����','FontSize',22)
set(x4,'Fontname','����','FontSize',22)
set(l4,'Fontname','����','FontSize',22)
set(gca,'FontSize',22);
grid on;
disp(['T1��ֵ:',num2str(mean(abs(timeCost)))])
disp(['T1����:',num2str(var(timeCost,1))])
disp(['T1���ֵ:',num2str(max(abs(timeCost)))])
disp(['T2��ֵ:',num2str(mean(abs(timeCostNoROI)))])
disp(['T2����:',num2str(var(timeCostNoROI,1))])
disp(['T2���ֵ:',num2str(max(abs(timeCostNoROI)))])