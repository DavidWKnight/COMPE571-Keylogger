clear all;
close all;

load('results.mat')

fprintf("Laptop Mean: %fms\n", mean(resultsLaptop.time));
fprintf("Membrane Mean: %fms\n", mean(resultsMembrane.time));
fprintf("Mechanical Mean: %fms\n", mean(resultsMechanical.time));

fprintf("Laptop Max: %fms\n", max(resultsLaptop.time));
fprintf("Membrane Max: %fms\n", max(resultsMembrane.time));
fprintf("Mechanical Max: %fms\n", max(resultsMechanical.time));

fprintf("Laptop Min: %fms\n", min(resultsLaptop.time));
fprintf("Membrane Min: %fms\n", min(resultsMembrane.time));
fprintf("Mechanical Min: %fms\n", min(resultsMechanical.time));
