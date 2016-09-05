function tracking(pixel)
% ��ȡ��Ƶͼ�������
rows = size(pixel,1);
% ��ȡ��Ƶͼ�������
cols = size(pixel,2);
% ��ȡ��Ƶ��֡��
frames = size(pixel,3);

% ѭ��������Ƶ�е�ÿһ֡�����ú�ɫ�����ѡ���˶�����
d = zeros(rows,cols,frames);
bw = zeros(rows,cols,frames);
for i = 2:frames
    % ���������֮֡��Ĳ�
    d(:,:,i)=(abs(pixel(:,:,i)-pixel(:,:,i-1)));
    % ����֡��ͼת��Ϊ��ֵͼ��
    k = d(:,:,i);
    bw(:,:,i) = im2bw(k, .2);
    % ��ע������ͼ���������ӵĲ���
    bwlabel(bw(:,:,i));
    % ȥ��ͼ���е�������
    bw(:,:,i) = medfilt2(bw(:,:,i));
    % ��ʾͼ��
    imshow(bw(:,:,i));
    
    % ������ͼ����
    hold on
    
    frtFound = 1;
    height = 0;
    top = 0;
    for h = 1:rows
        for w = 1:cols
            if(bw(h,w,i)>0.5)
                % ��¼�˶��������µ�λ��
                height = h;
                if(frtFound == 1)
                    % ��¼�˶��������ϵ�λ��
                    top = h;
                    frtFound = 0;
                end
                break;
            end
        end
    end

    frtFound = 1;
    width = 0;
    left = 0;
    for w = 1:cols
        for h = 1:rows
            if(bw(h,w,i)>0.5)
                % ��¼�˶��������ҵ�λ��
                width = w;
                if(frtFound == 1)
                    % ��¼�˶����������λ��
                    left = w;
                    frtFound = 0;
                end
                break;
            end
        end
    end

    % �����˶�����ĸ߶�
    realHeight = height - top;
    % �����˶�����Ŀ��
    realWidth = width - left;    

    % �˶��������ĵ�������
    heightCenter = top + realHeight / 2;
    % �˶��������ĵ������
    widthCenter = left + realWidth / 2;
    
    % �ú�ɫ���ο��ס�˶�����
    rectangle('Position',[left top realWidth realHeight],'EdgeColor','r');
    % �ں�ɫ���ο����Ļ���ɫ*�ű�־
    plot(widthCenter, heightCenter, 'r*');
    
    % ǿ�ƻ�ͼ
    drawnow;
    
    % �رռ�ͼ����
    hold off
end



