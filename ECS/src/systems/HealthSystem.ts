import { System } from '../System';
import { Health } from '../components/Health';

export class HealthSystem extends System {
    private entitiesToDestroy: number[] = [];

    constructor() {
        super();
        this.setRequiredComponents([Health]);
        this.priority = 50;
    }

    public update(deltaTime: number): void {
        const entities = this.getEntities();
        
        for (const entity of entities) {
            const health = this.world.getComponent(entity, Health);
            
            if (health && health.isDead()) {
                this.entitiesToDestroy.push(entity.id);
            }
        }
    }

    public postUpdate(deltaTime: number): void {
        for (const entityId of this.entitiesToDestroy) {
            this.world.destroyEntity(entityId);
        }
        this.entitiesToDestroy = [];
    }
}