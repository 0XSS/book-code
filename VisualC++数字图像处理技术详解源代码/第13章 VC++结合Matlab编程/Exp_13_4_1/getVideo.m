function pixel = getVideo
% ��ȡ��Ƶ�ļ�
avi = aviread('samplevideo.avi');
% �����Ƶ���ݲ�ת��cell����
video = {avi.cdata};
% ����������ת��Ϊʵ������
pixels = double(cat(4,video{1:2:end}))/255;
% �����Ƶ����
clear video

% ��ȡ��Ƶͼ�������
rows = size(pixels,1);
% ��ȡ��Ƶͼ�������
cols = size(pixels,2);
% ��ȡ��Ƶ��֡��
frames = size(pixels,4);

% ����Ƶ��ÿһ֡ͼ���ɲ�ɫתΪ�Ҷ�
pixel = zeros(rows,cols,frames);
for f = 1:frames
    pixel(:,:,f) = (rgb2gray(pixels(:,:,:,f)));
end