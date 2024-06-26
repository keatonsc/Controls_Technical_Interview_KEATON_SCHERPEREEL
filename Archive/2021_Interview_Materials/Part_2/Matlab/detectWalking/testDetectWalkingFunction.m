%% Test your code by running the following
close all;
clear all;
testDetectWalking();

function testDetectWalking()
    files = dir('dataset');
    files = files(3:end,1);

    for i = 1:size(files,1)
        filename = files(i).name;
        data_table = readtable(strcat('dataset/',filename));

        detectWalkingPredictions = detectWalking(data_table);

        figure
        plot(data_table.movement, 'b')
        hold on;
        plot(detectWalkingPredictions,'r')
    end
end
