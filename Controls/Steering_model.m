l=30;d=28;wd=9.5;ww=5.5;
%initial conditions
sth  = 0;            % steering rotation angle
d_th = 0;            % Rotation angle
th=0;
x=[0,0,1]';
Tr = eye(3);

for t=0:0.1:100
sth = steer(t);            % steering rotation angle
th  = th+sth*0.063;        % Rotation angle
Rot = [cos(th),sin(th); 
       -sin(th),cos(th);
       0,0];                % Rotation matrix
x  = x +[Rot,[0,0,1]']*[0,3,1]';  % Translation
Tr = [Rot,x];               % Transformation matrix


%Steering Rotation matrix;

SRot = [cos(sth),sin(sth),0;
         -sin(sth),cos(sth),0;
         0,0,1];


   
 c1=Tr*[d/2;l/2;1];    %Bot corner 1

%------------wheel1--------------
w1c1= Tr*([d/2;l/2;1]-SRot*[ww/2;wd/2;0]);
w1c2= Tr*([d/2;l/2;1]-SRot*[-ww/2;wd/2;0]);
w1c3= Tr*([d/2;l/2;1]-SRot*[-ww/2;-wd/2;0]);
w1c4= Tr*([d/2;l/2;1]-SRot*[ww/2;-wd/2;0]);
%---------------------------------

c2=Tr*[-d/2;l/2;1];   %Bot corner 2


%------------wheel2--------------
w2c1= Tr*([-d/2;l/2;1]-SRot*[ww/2;wd/2;0]);
w2c2= Tr*([-d/2;l/2;1]-SRot*[-ww/2;wd/2;0]);
w2c3= Tr*([-d/2;l/2;1]-SRot*[-ww/2;-wd/2;0]);
w2c4= Tr*([-d/2;l/2;1]-SRot*[ww/2;-wd/2;0]);
%---------------------------------
c3=Tr*[-d/2;-l/2;1];   %Bot corner 3

%------------wheel3--------------
w3c1= Tr*([-d/2;-l/2;1]-[ww/2;wd/2;0]);
w3c2= Tr*([-d/2;-l/2;1]-[-ww/2;wd/2;0]);
w3c3= Tr*([-d/2;-l/2;1]-[-ww/2;-wd/2;0]);
w3c4= Tr*([-d/2;-l/2;1]-[ww/2;-wd/2;0]);
%---------------------------------
c4=Tr*[d/2;-l/2;1];  %Bot corner 4


%------------wheel4--------------
w4c1= Tr*([d/2;-l/2;1]-[ww/2;wd/2;0]);
w4c2= Tr*([d/2;-l/2;1]-[-ww/2;wd/2;0]);
w4c3= Tr*([d/2;-l/2;1]-[-ww/2;-wd/2;0]);
w4c4= Tr*([d/2;-l/2;1]-[ww/2;-wd/2;0]);
%---------------------------------

com = Tr*[0,0,1]';

hold on;

axis([com(1)-100,com(1)+100,com(2)-100,com(2)+100]);
plt=zeros(20,1);
%Axles
plt(1)=plot([c1(1),c2(1)],[c1(2),c2(2)],'-','LineWidth',4,'Color','0,0,1');
plt(2)=plot([c3(1),c4(1)],[c3(2),c4(2)],'-','LineWidth',4,'Color','0,0,1');

plt(3)=plot([(c1(1)+c2(1))/2,(c3(1)+c4(1))/2],[(c1(2)+c2(2))/2,(c3(2)+c4(2))/2],...
    '-','LineWidth',3,'Color','0,0,1');

% Base frame 
plt(4)=plot([c1(1),c2(1),c3(1),c4(1),c1(1)],[c1(2),c2(2),c3(2),c4(2),c1(2)],...
    'o--','LineWidth',1,'Color','0,1,1');

%Wheels
plt(5)=plot([w1c1(1),w1c2(1),w1c3(1),w1c4(1),w1c1(1)],[w1c1(2),w1c2(2),w1c3(2),w1c4(2),w1c1(2)],...
    '-','LineWidth',2,'Color','0,0,0');
plt(6)=plot([w2c1(1),w2c2(1),w2c3(1),w2c4(1),w2c1(1)],[w2c1(2),w2c2(2),w2c3(2),w2c4(2),w2c1(2)],...
    '-','LineWidth',2,'Color','0,0,0');
plt(7)=plot([w3c1(1),w3c2(1),w3c3(1),w3c4(1),w3c1(1)],[w3c1(2),w3c2(2),w3c3(2),w3c4(2),w3c1(2)],...
    '-','LineWidth',2,'Color','0,0,0');
plt(8)=plot([w4c1(1),w4c2(1),w4c3(1),w4c4(1),w4c1(1)],[w4c1(2),w4c2(2),w4c3(2),w4c4(2),w4c1(2)],...
    '-','LineWidth',2,'Color','0,0,0');


plt(9)=plot(com(1),com(2),'o','MarkerSize',5,'MarkerFaceColor','1,0,0');
axis equal;
grid on;
%trajectories

if(t==0)
    comp=com;
    c1p=c1;
    c2p=c2;
    c3p=c3;
    c4p=c4;
end

plot([comp(1),com(1)],[comp(2),com(2)],'-','MarkerSize',1,'Color','1,0,0');
plot([c1p(1),c1(1)],[c1p(2),c1(2)],'-','MarkerSize',1,'Color','0,0,1');
plot([c2p(1),c2(1)],[c2p(2),c2(2)],'-','MarkerSize',1,'Color','0,0,1');
plot([c3p(1),c3(1)],[c3p(2),c3(2)],'-','MarkerSize',1,'Color','0,1,0');
plot([c4p(1),c4(1)],[c4p(2),c4(2)],'-','MarkerSize',1,'Color','0,1,0');



pause(0.1);
delete(plt);

comp=com;
c1p=c1;
c2p=c2;
c3p=c3;
c4p=c4;

end

function sth = steer(t)
sth = pi/180*40*cos(0.9*t)*exp(-0.05*t);
end
%close all;