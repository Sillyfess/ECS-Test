import { System } from '../System';
import { Transform } from '../components/Transform';
import { Velocity } from '../components/Velocity';

export class BoundsSystem extends System {
    private bounds = {
        minX: -1000,
        maxX: 1000,
        minY: -1000,
        maxY: 1000,
        minZ: -1000,
        maxZ: 1000
    };

    constructor() {
        super();
        this.setRequiredComponents([Transform, Velocity]);
        this.priority = 90;
    }

    public setBounds(minX: number, maxX: number, minY: number, maxY: number, minZ: number, maxZ: number): void {
        this.bounds = { minX, maxX, minY, maxY, minZ, maxZ };
    }

    public update(deltaTime: number): void {
        const entities = this.getEntities();
        
        for (const entity of entities) {
            const transform = this.world.getComponent(entity, Transform);
            const velocity = this.world.getComponent(entity, Velocity);
            
            if (transform && velocity) {
                if (transform.x <= this.bounds.minX || transform.x >= this.bounds.maxX) {
                    velocity.dx = -velocity.dx;
                    transform.x = Math.max(this.bounds.minX, Math.min(this.bounds.maxX, transform.x));
                }
                
                if (transform.y <= this.bounds.minY || transform.y >= this.bounds.maxY) {
                    velocity.dy = -velocity.dy;
                    transform.y = Math.max(this.bounds.minY, Math.min(this.bounds.maxY, transform.y));
                }
                
                if (transform.z <= this.bounds.minZ || transform.z >= this.bounds.maxZ) {
                    velocity.dz = -velocity.dz;
                    transform.z = Math.max(this.bounds.minZ, Math.min(this.bounds.maxZ, transform.z));
                }
            }
        }
    }
}