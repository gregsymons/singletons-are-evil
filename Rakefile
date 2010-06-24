DIAGRAMS = FileList['diagrams/*.dia']
IMAGES_DIR = 'img'
IMAGE_FILES = DIAGRAMS.pathmap("#{IMAGES_DIR}/%n.png")

rule '.png' => lambda{ |png| find_diagram(png) } do |t|
    puts "Transforming #{t.name} to #{t.source}"
    sh "dia -n --export=#{t.name} #{t.source}" 
end

task :default => :images

task :images => IMAGES_DIR
task :images => IMAGE_FILES

directory IMAGES_DIR

def find_diagram(png)
    base = File.basename(png, '.png')
    DIAGRAMS.find { |diagram| File.basename(diagram, '.dia') == base }
end
