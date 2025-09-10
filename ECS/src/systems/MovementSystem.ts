import { System } from '../System';
import { Transform } from '../components/Transform';
import { Velocity } from '../components/Velocity';

export class MovementSystem extends System {
    constructor() {
        super();
        this.setRequiredComponents([Transform, Velocity]);
        this.priority = 100;
    }

    public update(deltaTime: number): void {
        const entities = this.getEntities();
        
        for (const entity of entities) {
            const transform = this.world.getComponent(entity, Transform);
            const velocity = this.world.getComponent(entity, Velocity);
            
            if (transform && velocity) {
                transform.x += velocity.dx * deltaTime;
                transform.y += velocity.dy * deltaTime;
                transform.z += velocity.dz * deltaTime;
            }
        }
    }
}